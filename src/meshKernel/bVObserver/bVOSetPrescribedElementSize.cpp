/*---------------------------------------------------------------------------*\
  dtOO < design tool Object-Oriented >
    
    Copyright (C) 2024 A. Tismer.
-------------------------------------------------------------------------------
License
    This file is part of dtOO.

    dtOO is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the LICENSE.txt file in the
    dtOO root directory for more details.

    You should have received a copy of the License along with dtOO.

\*---------------------------------------------------------------------------*/

#include "bVOSetPrescribedElementSize.h"

#include <logMe/logMe.h>
#include <xmlHeaven/dtXmlParserBase.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <analyticFunctionHeaven/scaOneDPolyInterface.h>
#include <analyticGeometryHeaven/map1dTo3d.h>
#include <boundedVolume.h>
#include <meshEngine/dtGmshModel.h>
#include <meshEngine/dtGmshEdge.h>

#include <attributionHeaven/scaOneDPolyIElSize.h>
#include <gslMinFloatAttr.h>
#include <interfaceHeaven/staticPropertiesHandler.h>
#include "bVOInterfaceFactory.h"
#include <interfaceHeaven/stringPrimitive.h>
#include <logMe/dtMacros.h>

#include <boost/assign.hpp>

namespace dtOO {  
  bool bVOSetPrescribedElementSize::_registrated 
  =
  bVOInterfaceFactory::registrate(
    dt__tmpPtr(
      bVOSetPrescribedElementSize, new bVOSetPrescribedElementSize()
    )
  );
  
  bVOSetPrescribedElementSize::bVOSetPrescribedElementSize() {
  }

  bVOSetPrescribedElementSize::~bVOSetPrescribedElementSize() {
    
  }
 
  std::vector< std::string > 
  bVOSetPrescribedElementSize::factoryAlias( void ) const {
    return ::boost::assign::list_of
      ("bVOSetPrescribedFirstElementSize")
      ("bVOSetPrescribedLastElementSize");
  }


  void bVOSetPrescribedElementSize::jInit( 
    jsonPrimitive const & jE,
		baseContainer const * const bC,
    lvH_constValue const * const cV,
    lvH_analyticFunction const * const aF,
    lvH_analyticGeometry const * const aG,
    lvH_boundedVolume const * const bV,
    boundedVolume * attachTo
  ) {
    bVOInterface::jInit(jE, bC, cV, aF, aG, bV, attachTo);

    const scaOneDPolyInterface & s1dPoly
    =
    scaOneDPolyInterface::ConstRefCast(
      jE.lookupRef< analyticFunction >( "_grading", aF )
    );
    dt__throwIf( !s1dPoly.isCompound(), init() );
    _grading
    =
    analyticFunctionCompoundTrojanHorse< scaOneDPolyInterface >(
      &s1dPoly, s1dPoly.vecRef(), s1dPoly.mapRef()
    );
    _polyI.reset( scaOneDPolyInterface::MustDownCast( s1dPoly.weakClone() ) );

    dt__throwIf( (_polyI->nDOF()!=1) && (_polyI->nDOF()!=2), jInit() );
  }  
 
  void bVOSetPrescribedElementSize::init( 
		::QDomElement const & element,
		baseContainer const * const bC,
		lvH_constValue const * const cV,
		lvH_analyticFunction const * const aF,
		lvH_analyticGeometry const * const aG,
		lvH_boundedVolume const * const bV,
		boundedVolume * attachTo
  ) {
    // <bVObserver 
    // 	name="bVOSetPrescribedElementSize"
    // 	type="7"
    // 	gradingLabel="aF_dt_gridGradingSpline_7"
    // 	firstElementSize="0.001"
    // 	lastElementSize="0.001"
    // />					
    //
    // init bVOInterface
    //
    bVOInterface::init(element, bC, cV, aF, aG, bV, attachTo);
    
    jsonPrimitive jE;
    jE.append< dtInt >("_type",qtXmlBase::getAttributeInt("type", element));
    jE.append< dtReal >(
      "_firstElementSize",
      dtXmlParserBase::getAttributeFloatMuParse(
        "firstElementSize", element, cV, aF
      )
    );
    if ( dtXmlParserBase::hasAttribute("lastElementSize", element) ) {
      jE.append< dtReal >(
        "_lastElementSize",
        dtXmlParserBase::getAttributeFloatMuParse(
          "lastElementSize", element, cV, aF
        )
      );
    }   
    jE.append< analyticFunction const * >("_grading",
      aF->get( qtXmlBase::getAttributeStr("gradingLabel", element) ) 
    );
    bVOSetPrescribedElementSize::jInit(jE, bC, cV, aF, aG, bV, attachTo);
  }
  
  void bVOSetPrescribedElementSize::preUpdate( void ) {
		dt__ptrAss(dtGmshModel * gm, ptrBoundedVolume()->getModel());
		
    //
    // set current model
    //
    ::GModel::setCurrent(gm);
    
    logContainer< bVOSetPrescribedElementSize > logC(
      logINFO, "preUpdate()"
    );

    dt__forAllRefAuto( gm->dtEdges(), aEdge) {
      if ( 
        aEdge->meshAttributes.typeTransfinite 
        == 
        config().lookup< dtInt >("_type") 
      ) {
        
        //
        // reset pointer to new clone
        //
        _polyI.reset( 
          scaOneDPolyInterface::MustDownCast( _polyI->weakClone() ) 
        );
      
        //
        // get edge length
        //
        dtReal ll = aEdge->getMap1dTo3d()->length();

        //
        // x value of checkpoint of function
        //
        dtReal x0 = 1./(aEdge->meshAttributes.nbPointsTransfinite-1);
        dtReal x1 = 1. - 1./(aEdge->meshAttributes.nbPointsTransfinite-1);
        
        dtReal lByS0 = config().lookupDef< dtReal >("_firstElementSize", 0.0);
        if (lByS0>0.0) lByS0 = ll/lByS0;
        dtReal lByS1 = config().lookupDef< dtReal >("_lastElementSize", 0.0);
        if (lByS1>0.0) lByS1 = ll/lByS1;

        gslMinFloatAttr md(
          new scaOneDPolyIElSize( 
            _polyI.get(), x0, x1, lByS0, lByS1
          ),
          ::std::vector<dtReal>(_polyI->nDOF(), 0.5),
          ::std::vector<dtReal>(_polyI->nDOF(), 0.5),
          1.0E-08
        );
        md.perform();
        //std::vector< dtReal > const & theRoot = md.result();

        //
        // output
        //
        if ( config().contains("_firstElementSize") ) {
          logC() 
            << logMe::dtFormat(
              "Edge %4d (firstElementSize = %16.10e): %16.10e -> %6.1f %%"
            ) 
              % aEdge->tag()
              % config().lookup< dtReal >("_firstElementSize")
              % (_polyI->YFloat( x0 ) * ll)
              % ( 
                  100. * _polyI->YFloat( x0 ) * ll 
                  / 
                  config().lookup< dtReal >("_firstElementSize")
                ) 
            << std::endl
            << "DOF = " << md.result() << std::endl;
        }
        if ( config().contains("_lastElementSize") ) {
          logC() 
            << logMe::dtFormat(
              "Edge %4d (lastElementSize = %16.10e): %16.10e -> %6.1f %%"
            ) 
              % aEdge->tag()
              % config().lookup< dtReal >("_lastElementSize")
              % ( (1.-_polyI->YFloat( x1) ) * ll )
              % ( 
                  100. * (1.-_polyI->YFloat( x1 )) * ll 
                  / 
                  config().lookup< dtReal >("_lastElementSize")
                ) 
            << std::endl
            << "DOF = " << md.result() << std::endl;
        }

        //
        // add component
        //
        _grading.addComponent( _polyI.get(), aEdge->tag() );          
      }
    }
  }
}
