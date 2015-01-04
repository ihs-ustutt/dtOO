#include "dtXmlParserFunctionDecoratorFactory.h"

#include <logMe/logMe.h>
#include <string.h>
#include "aFBuilder/baseContainerFunctionDecorator.h"
#include "aFBuilder/scaMuParserOneDFunctionDecorator.h"
#include "aFBuilder/scaCurve2dOneDFunctionDecorator.h"
#include "aFBuilder/sca3PointMeanlineFunctionDecorator.h"
#include "aFBuilder/analyticFunctionFunctionDecorator.h"
#include "aFBuilder/vec2dCurve2dOneDFunctionDecorator.h"
#include "aFBuilder/vec3dSurfaceTwoDFunctionDecorator.h"
#include "aFBuilder/vec3dCurveOneDFunctionDecorator.h"
#include "aFBuilder/vec3dTwoDFiveRectangleFunctionDecorator.h"
#include "aFBuilder/vec3dTwoDTwelveRectangleFunctionDecorator.h"
#include "aFBuilder/vec3dTransVolThreeDFunctionDecorator.h"
#include "aFBuilder/bSplineSurface2d_bSplineCurve2dFillConstructOCCFunctionDecorator.h"
#include "aFBuilder/vec3dMuParserThreeDFunctionDecorator.h"
#include "aFBuilder/scaMuParserTwoDFunctionDecorator.h"
#include "aFBuilder/muParserFunctionDecorator.h"

#define __IFRET(className) \
    if ( strcmp(str, #className) == 0 ) { \
      return new className; \
    }
#define __IFRETCUSTOM(className, retClass) \
  if ( strcmp(str, #className) == 0 ) { \
    return new retClass; \
  }

namespace dtOO {
  dtXmlParserFunctionDecoratorFactory::dtXmlParserFunctionDecoratorFactory() {
  }

  dtXmlParserFunctionDecoratorFactory::~dtXmlParserFunctionDecoratorFactory() {
  }

  dtXmlParserFunctionDecorator * 
	dtXmlParserFunctionDecoratorFactory::create(char const * const str) {
    DTINFOWF(create(), << "creating " << str <<  "...");

    __IFRET(baseContainerFunctionDecorator);		
		__IFRETCUSTOM(baseContainer, baseContainerFunctionDecorator);
    __IFRET(scaMuParserOneDFunctionDecorator);
    __IFRETCUSTOM(scaMuParserOneD, scaMuParserOneDFunctionDecorator);
    __IFRET(scaCurve2dOneDFunctionDecorator);
    __IFRETCUSTOM(scaCurve2dOneD, scaCurve2dOneDFunctionDecorator);
    __IFRET(sca3PointMeanlineFunctionDecorator);
    __IFRETCUSTOM(sca3PointMeanline, sca3PointMeanlineFunctionDecorator);    
    __IFRET(analyticFunctionFunctionDecorator);
    __IFRETCUSTOM(analyticFunction, analyticFunctionFunctionDecorator);  
		__IFRETCUSTOM(scaFunction, analyticFunctionFunctionDecorator);  
		__IFRETCUSTOM(vec2dFunction, analyticFunctionFunctionDecorator);  
		__IFRET(vec2dCurve2dOneDFunctionDecorator);
		__IFRETCUSTOM(vec2dCurve2dOneD, vec2dCurve2dOneDFunctionDecorator);
		__IFRET(vec3dSurfaceTwoDFunctionDecorator);
		__IFRETCUSTOM(vec3dSurfaceTwoD, vec3dSurfaceTwoDFunctionDecorator);
		__IFRET(vec3dCurveOneDFunctionDecorator);
		__IFRETCUSTOM(vec3dCurveOneD, vec3dCurveOneDFunctionDecorator);
		__IFRET(vec3dTwoDFiveRectangleFunctionDecorator);
		__IFRETCUSTOM(vec3dTwoDFiveRectangle, vec3dTwoDFiveRectangleFunctionDecorator);
		__IFRET(vec3dTwoDTwelveRectangleFunctionDecorator);
		__IFRETCUSTOM(vec3dTwoDTwelveRectangle, vec3dTwoDTwelveRectangleFunctionDecorator);
		__IFRET(vec3dTransVolThreeDFunctionDecorator);
		__IFRETCUSTOM(vec3dTransVolThreeD, vec3dTransVolThreeDFunctionDecorator);
		__IFRET(bSplineSurface2d_bSplineCurve2dFillConstructOCCFunctionDecorator);
		__IFRETCUSTOM(
		  bSplineSurface2d_bSplineCurve2dFillConstructOCC, 
			bSplineSurface2d_bSplineCurve2dFillConstructOCCFunctionDecorator
		);
		__IFRET(vec3dMuParserThreeDFunctionDecorator);
		__IFRETCUSTOM(vec3dMuParserThreeD, vec3dMuParserThreeDFunctionDecorator);
		__IFRET(scaMuParserTwoDFunctionDecorator);
		__IFRETCUSTOM(scaMuParserTwoD, scaMuParserTwoDFunctionDecorator);
		__IFRET(muParserFunctionDecorator);
		__IFRETCUSTOM(muParser, muParserFunctionDecorator);
    
    dt__THROW(create(), << str <<  " could not be created.");
  }

  dtXmlParserFunctionDecorator * 
	dtXmlParserFunctionDecoratorFactory::create( std::string const str ) {
    return create( str.c_str() );
  }
}
