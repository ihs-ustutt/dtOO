#include "analyticFunctionCombinationAFXmlBuilder.h"

#include <xmlHeaven/aFXmlBuilderFactory.h>
#include <xmlHeaven/dtXmlParserBase.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <analyticFunctionHeaven/vec3dThreeD.h>
#include <analyticFunctionHeaven/vec2dTwoD.h>
#include <analyticFunctionHeaven/analyticFunctionCombination.h>
#include <logMe/logMe.h>

#include <QtXml/QDomElement>

namespace dtOO {
  bool analyticFunctionCombinationAFXmlBuilder::_registrated 
  =
  aFXmlBuilderFactory::registrate(
    dt__tmpPtr(
      analyticFunctionCombinationAFXmlBuilder, 
      new analyticFunctionCombinationAFXmlBuilder()
    )
  );
  
  analyticFunctionCombinationAFXmlBuilder
    ::analyticFunctionCombinationAFXmlBuilder(
  ) {
    
  }

  analyticFunctionCombinationAFXmlBuilder
    ::~analyticFunctionCombinationAFXmlBuilder(
  ) {
    
  }

  void analyticFunctionCombinationAFXmlBuilder::buildPart(
	  ::QDomElement const & toBuildP, 
		baseContainer * const bC,
		cVPtrVec const * const cV, 
		aFPtrVec const * const aF,
		aFPtrVec * ret
	) const {
    //
    // clone functions
    //
    dt__pH(analyticFunction) yy(
      dtXmlParserBase::createAnalyticFunction(
        dtXmlParserBase::getChild("y", toBuildP), bC, cV, aF
      )
    );
    dt__pH(analyticFunction) yyInv(
      dtXmlParserBase::createAnalyticFunction(
        dtXmlParserBase::getChild("invY", toBuildP), bC, cV, aF
      )
    );    

    //
    // create combination
    //
    if ( vec3dThreeD::Is(yy.get()) && vec3dThreeD::Is(yyInv.get()) ) {
      ret->push_back( 
        new analyticFunctionCombination< vec3dThreeD, vec3dThreeD >( 
          vec3dThreeD::ConstSecureCast(yy.get()), 
          vec3dThreeD::ConstSecureCast(yyInv.get()) 
        )      
      );
    }
    else if ( vec2dTwoD::Is(yy.get()) && vec2dTwoD::Is(yyInv.get()) ) {
      ret->push_back( 
        new analyticFunctionCombination< vec2dTwoD, vec2dTwoD >( 
          vec2dTwoD::ConstSecureCast(yy.get()), 
          vec2dTwoD::ConstSecureCast(yyInv.get()) 
        )      
      );
    }    
    else dt__throwUnexpected(buildPart());
    

  }
}