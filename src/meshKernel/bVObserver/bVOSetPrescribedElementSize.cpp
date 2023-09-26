#include "bVOSetPrescribedElementSize.h"

#include <logMe/logMe.h>
#include <xmlHeaven/dtXmlParserBase.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <analyticFunctionHeaven/scaOneDPolyInterface.h>
#include <analyticGeometryHeaven/map1dTo3d.h>
#include <boundedVolume.h>
#include <meshEngine/dtGmshModel.h>
#include <meshEngine/dtGmshEdge.h>

#include <dtAnalysis.h>
#include <Math/Functor.h>
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
        _ll = aEdge->getMap1dTo3d()->length();

        //
        // x value of checkpoint of function
        //
        _checkXFirst = 1./(aEdge->meshAttributes.nbPointsTransfinite-1);
        _checkXLast = 1. - 1./(aEdge->meshAttributes.nbPointsTransfinite-1);
        std::vector< dtReal > theRoot = this->perform(
          aEdge, 
          &bVOSetPrescribedElementSize::FFirstLast
        );      

        if ( config().contains("_firstElementSize") ) {
          logC() 
            << logMe::dtFormat(
              "Edge %4d (firstElementSize = %16.10e): %16.10e -> %6.1f %%"
            ) 
              % aEdge->tag()
              % config().lookup< dtReal >("_firstElementSize")
              % (_polyI->YFloat( _checkXFirst ) * _ll)
              % ( 
                  100. * _polyI->YFloat( _checkXFirst ) * _ll 
                  / 
                  config().lookup< dtReal >("_firstElementSize")
                ) 
            << std::endl
            << "DOF = " << theRoot << std::endl;
        }
        if ( config().contains("_lastElementSize") ) {
          logC() 
            << logMe::dtFormat(
              "Edge %4d (lastElementSize = %16.10e): %16.10e -> %6.1f %%"
            ) 
              % aEdge->tag()
              % config().lookup< dtReal >("_lastElementSize")
              % ( (1.-_polyI->YFloat( _checkXLast) ) * _ll )
              % ( 
                  100. * (1.-_polyI->YFloat( _checkXLast )) * _ll 
                  / 
                  config().lookup< dtReal >("_lastElementSize")
                ) 
            << std::endl
            << "DOF = " << theRoot << std::endl;
        }
        _grading.addComponent(
          _polyI.get(), aEdge->tag()
        );          
      }
    }
  }
  
	double bVOSetPrescribedElementSize::FFirstLast( double const * xx ) {
    std::vector< dtReal > xVec(_polyI->nDOF(), -1);
    dt__forAllIndex(xVec, ii) xVec[ii] = xx[ii];
    _polyI->setDOF(xVec);
   
    double retFirst = 0.0;
    double retLast = 0.0;
    if ( config().contains("_firstElementSize") ) {
      retFirst
      =
      fabs(
        _polyI->YFloat(_checkXFirst) * _ll 
        - 
        config().lookup< dtReal >("_firstElementSize")
      )
      /
      config().lookup< dtReal >("_firstElementSize");
    }
    if ( config().contains("_lastElementSize") ) {
      retLast
      =
      fabs(
        (1.-_polyI->YFloat(_checkXLast)) * _ll 
        - 
        config().lookup< dtReal >("_lastElementSize")
      )
      /
      config().lookup< dtReal >("_lastElementSize");
    }
    dt__debug(
      FFirstLast(), 
      << "Difference : " << retFirst+retLast 
      << " = ( " << 100.*retFirst << " %, " << 100.*retLast << " %) "
    );
    return retFirst + retLast;
	}

  std::vector< dtReal > bVOSetPrescribedElementSize::perform(
    dtGmshEdge * aEdge,
    double (bVOSetPrescribedElementSize::*fPtr)(double const * xx) 
  ) {
    // 
    // multidimensional minimization
    //
    dt__pH(dtMinimizer) min(
      dtAnalysis::createMinimizer(":Minuit2::kMigrad:")
    );
    ::ROOT::Math::Functor toMin(
      this, 
      fPtr,
      _polyI->nDOF()
    );			
    min->SetFunction(toMin);  

    //
    // set bounds
    //
    dt__forFromToIndex(0, _polyI->nDOF(), ii) {
      min->SetVariable( ii, "V_"+stringPrimitive::intToString(ii), 0.5, 0.01 );
      min->SetVariableLimits(ii, 0., 1.);
    }
    //
    // minimizer options
    //        
    min->SetMaxFunctionCalls(1000000);
    min->SetMaxIterations(1000);
    min->SetTolerance( 1.e-8 );			
    min->SetPrintLevel(
      staticPropertiesHandler::getInstance()->getOptionInt(
        "root_printLevel"
      ) 
    );

    //
    // minimize
    //
    min->Minimize();    

    double const * const theRoot = min->X();
    std::vector< dtReal > ret(_polyI->nDOF(), -1.0);
    dt__forFromToIndex(0, _polyI->nDOF(), ii) {
      ret[ii] = static_cast<dtReal>(theRoot[ii]); 
    }
    return ret; 
  }
}
