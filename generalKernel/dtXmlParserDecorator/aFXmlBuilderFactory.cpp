#include "aFXmlBuilderFactory.h"

#include <logMe/logMe.h>
#include <string.h>
#include "aFXmlBuilder/baseContainerAFXmlBuilder.h"
#include "aFXmlBuilder/scaMuParserOneDAFXmlBuilder.h"
#include "aFXmlBuilder/scaCurve2dOneDAFXmlBuilder.h"
#include "aFXmlBuilder/sca3PointMeanlineAFXmlBuilder.h"
#include "aFXmlBuilder/sca5PointMeanlineAFXmlBuilder.h"
#include "aFXmlBuilder/analyticFunctionAFXmlBuilder.h"
#include "aFXmlBuilder/vec2dCurve2dOneDAFXmlBuilder.h"
#include "aFXmlBuilder/vec3dSurfaceTwoDAFXmlBuilder.h"
#include "aFXmlBuilder/vec3dCurveOneDAFXmlBuilder.h"
#include "aFXmlBuilder/vec3dTwoDFiveRectangleAFXmlBuilder.h"
#include "aFXmlBuilder/vec3dTwoDTwelveRectangleAFXmlBuilder.h"
#include "aFXmlBuilder/vec3dTransVolThreeDAFXmlBuilder.h"
#include "aFXmlBuilder/bSplineSurface2d_bSplineCurve2dFillConstructOCCAFXmlBuilder.h"
#include "aFXmlBuilder/vec3dMuParserThreeDAFXmlBuilder.h"
#include "aFXmlBuilder/scaMuParserTwoDAFXmlBuilder.h"
#include "aFXmlBuilder/muParserAFXmlBuilder.h"

#define __IFRET(className) \
    if ( strcmp(str, #className) == 0 ) { \
      return new className; \
    }
#define __IFRETCUSTOM(className, retClass) \
  if ( strcmp(str, #className) == 0 ) { \
    return new retClass; \
  }

namespace dtOO {
  aFXmlBuilderFactory::aFXmlBuilderFactory() {
  }

  aFXmlBuilderFactory::~aFXmlBuilderFactory() {
  }

  aFXmlBuilder * aFXmlBuilderFactory::create(char const * const str) {
    dt__info(create(), << "creating " << str <<  "...");

    __IFRET(baseContainerAFXmlBuilder);		
		__IFRETCUSTOM(baseContainer, baseContainerAFXmlBuilder);
    __IFRET(scaMuParserOneDAFXmlBuilder);
    __IFRETCUSTOM(scaMuParserOneD, scaMuParserOneDAFXmlBuilder);
    __IFRET(scaCurve2dOneDAFXmlBuilder);
    __IFRETCUSTOM(scaCurve2dOneD, scaCurve2dOneDAFXmlBuilder);
    __IFRET(sca3PointMeanlineAFXmlBuilder);
    __IFRETCUSTOM(sca3PointMeanline, sca3PointMeanlineAFXmlBuilder);    
    __IFRET(sca5PointMeanlineAFXmlBuilder);
    __IFRETCUSTOM(sca5PointMeanline, sca5PointMeanlineAFXmlBuilder);    
    __IFRET(analyticFunctionAFXmlBuilder);
    __IFRETCUSTOM(analyticFunction, analyticFunctionAFXmlBuilder);  
		__IFRETCUSTOM(scaFunction, analyticFunctionAFXmlBuilder);  
		__IFRETCUSTOM(vec2dFunction, analyticFunctionAFXmlBuilder);  
		__IFRET(vec2dCurve2dOneDAFXmlBuilder);
		__IFRETCUSTOM(vec2dCurve2dOneD, vec2dCurve2dOneDAFXmlBuilder);
		__IFRET(vec3dSurfaceTwoDAFXmlBuilder);
		__IFRETCUSTOM(vec3dSurfaceTwoD, vec3dSurfaceTwoDAFXmlBuilder);
		__IFRET(vec3dCurveOneDAFXmlBuilder);
		__IFRETCUSTOM(vec3dCurveOneD, vec3dCurveOneDAFXmlBuilder);
		__IFRET(vec3dTwoDFiveRectangleAFXmlBuilder);
		__IFRETCUSTOM(vec3dTwoDFiveRectangle, vec3dTwoDFiveRectangleAFXmlBuilder);
		__IFRET(vec3dTwoDTwelveRectangleAFXmlBuilder);
		__IFRETCUSTOM(vec3dTwoDTwelveRectangle, vec3dTwoDTwelveRectangleAFXmlBuilder);
		__IFRET(vec3dTransVolThreeDAFXmlBuilder);
		__IFRETCUSTOM(vec3dTransVolThreeD, vec3dTransVolThreeDAFXmlBuilder);
		__IFRET(bSplineSurface2d_bSplineCurve2dFillConstructOCCAFXmlBuilder);
		__IFRETCUSTOM(
		  bSplineSurface2d_bSplineCurve2dFillConstructOCC, 
			bSplineSurface2d_bSplineCurve2dFillConstructOCCAFXmlBuilder
		);
		__IFRET(vec3dMuParserThreeDAFXmlBuilder);
		__IFRETCUSTOM(vec3dMuParserThreeD, vec3dMuParserThreeDAFXmlBuilder);
		__IFRET(scaMuParserTwoDAFXmlBuilder);
		__IFRETCUSTOM(scaMuParserTwoD, scaMuParserTwoDAFXmlBuilder);
		__IFRET(muParserAFXmlBuilder);
		__IFRETCUSTOM(muParser, muParserAFXmlBuilder);
    
    dt__throw(create(), << str <<  " could not be created.");
  }

  aFXmlBuilder * aFXmlBuilderFactory::create( std::string const str ) {
    return create( str.c_str() );
  }
}
