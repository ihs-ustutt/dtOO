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

#include "vec3dThreeD_transVolAFXmlBuilder.h"

#include <xmlHeaven/aFXmlBuilderFactory.h>
#include <xmlHeaven/dtXmlParserBase.h>
#include <analyticFunctionHeaven/aFBuilder/vec3dTransVolThreeD_skinBSplineSurfaces.h>
#include <analyticFunctionHeaven/vec3dTransVolThreeD.h>
#include <analyticFunctionHeaven/vec3dSurfaceTwoD.h>

#include <boost/assign.hpp>
#include <QtXml/QDomElement>

namespace dtOO {
  bool vec3dThreeD_transVolAFXmlBuilder::_registrated 
  =
  aFXmlBuilderFactory::registrate(
    dt__tmpPtr(
      vec3dThreeD_transVolAFXmlBuilder, new vec3dThreeD_transVolAFXmlBuilder()
    )
  );
  
  vec3dThreeD_transVolAFXmlBuilder::vec3dThreeD_transVolAFXmlBuilder() {
  }

  vec3dThreeD_transVolAFXmlBuilder::~vec3dThreeD_transVolAFXmlBuilder() {

  }

  std::vector< std::string > vec3dThreeD_transVolAFXmlBuilder::factoryAlias( 
  void 
  ) const {
    return ::boost::assign::list_of
      ("vec3dTransVolThreeD")
      ("vec3dTransVolThreeDAFXmlBuilder");
  }
  
  void vec3dThreeD_transVolAFXmlBuilder::buildPart(
		::QDomElement const & toBuildP, 
		baseContainer * const bC,
		lvH_constValue const * const cValP, 
		lvH_analyticFunction const * const depSFunP,
		lvH_analyticFunction * sFunP 
	) const {
    dt__throwIf(
      !dtXmlParserBase::hasChild("analyticFunction", toBuildP),
      buildPart()
    );
    
    std::vector< ::QDomElement > eVec 
    = 
    dtXmlParserBase::getChildVector("analyticFunction", toBuildP);
    
    //
    // set input
    //
    vectorHandling< analyticFunction const * > aF(eVec.size());
    dt__forAllIndex(eVec, ii) {
      aF[ii]
      =
      dtXmlParserBase::createAnalyticFunction(eVec[ii], bC, cValP, depSFunP);
    }
    
    sFunP->push_back(
      vec3dTransVolThreeD_skinBSplineSurfaces( &aF ).result()
    );
  }

  void vec3dThreeD_transVolAFXmlBuilder::buildPartCompound(
	  ::QDomElement const & toBuildP, 
	  baseContainer * const bC,
    lvH_constValue const * const cValP, 
		lvH_analyticFunction const * const depSFunP,
		lvH_analyticFunction * sFunP
	) const {
    dt__throwIf(
      !dtXmlParserBase::hasChild("analyticFunction", toBuildP), 
      buildPartCompound()
    );
    
    ::QDomElement elementP 
    = 
    dtXmlParserBase::getChild("analyticFunction", toBuildP);
    //
    // set input
    //
    vectorHandling< vec3dSurfaceTwoDCompound const * > v3dSC;
    while ( !elementP.isNull() ) {
      analyticFunction * aF 
      = 
      dtXmlParserBase::createAnalyticFunction( 
        &elementP, bC, cValP, depSFunP 
      );
      if ( !vec3dSurfaceTwoDCompound::ConstDownCast(aF) ) {
        dt__throw(
          buildPartCompound(), 
          << dt__eval(
            vec3dSurfaceTwoDCompound::ConstDownCast(aF)
          ) << std::endl
          << dtXmlParserBase::getAttributeStr("label", elementP) 
          << " has not correct type." 
        );
      }				
      v3dSC.push_back( vec3dSurfaceTwoDCompound::ConstSecureCast(aF) );
      elementP 
      = 
      dtXmlParserBase::getNextSibling("analyticFunction", elementP);      
    }

    dtInt nComponents = v3dSC[0]->nComponents();
    vec3dTransVolThreeDCompound * vec3dTVC 
    =
    new vec3dTransVolThreeDCompound();
    for (int cc=0; cc<nComponents; cc++) {
      vectorHandling< analyticFunction const * > iAF;
      dt__forAllIndex(v3dSC, ii) {
        iAF.push_back( v3dSC[ii]->component(cc).clone() );
      }
      //
      // create vec3dSurfaceTwoD
      //
      dt__pH(vec3dTransVolThreeD) tv(
        vec3dTransVolThreeD_skinBSplineSurfaces(&iAF).result()
      );
      vec3dTVC->addComponent( *tv );
      iAF.destroy();
    }

    sFunP->push_back(vec3dTVC);
  }	
}
