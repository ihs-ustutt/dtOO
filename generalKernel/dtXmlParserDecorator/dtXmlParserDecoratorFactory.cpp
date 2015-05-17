#include "dtXmlParserDecoratorFactory.h"

#include "dtXmlParserDecorator.h"
#include <logMe/logMe.h>
#include <string.h>
#include "aGXmlBuilder/baseContainerDecorator.h"
#include "aGXmlBuilder/analyticGeometryDecorator.h"
#include "aGXmlBuilder/trans6SidedCubeDecorator.h"
#include "aGXmlBuilder/scaInMap1dTo3dDecorator.h"
#include "aGXmlBuilder/vec2dInMap2dTo3dDecorator.h"
#include "aGXmlBuilder/vec3dInMap3dTo3dDecorator.h"
#include "aGXmlBuilder/rotatingMap2dTo3dDecorator.h"
#include "aGXmlBuilder/infinityMap3dTo3dDecorator.h"
#include "aGXmlBuilder/makeCompoundDecorator.h"
#include "aGXmlBuilder/vec3dInMap3dTo3dSurroundingInternalDecorator.h"
#include "aGXmlBuilder/map2dTo3dApproximateInMap3dTo3dDecorator.h"
#include "aGXmlBuilder/bezierCurve_pointConstructOCCDecorator.h"
#include "aGXmlBuilder/bSplineCurve_curveConnectConstructOCCDecorator.h"
#include "aGXmlBuilder/bSplineSurface_skinConstructOCCDecorator.h"
#include "aGXmlBuilder/surfaceOfRevolution_curveRotateConstructOCCDecorator.h"
#include "aGXmlBuilder/bezierCurvePointPickFromPartConstructOCCDecorator.h"
#include "aGXmlBuilder/bSplineCurve_pointPickFromPartConstructOCCDecorator.h"
#include "aGXmlBuilder/bezierSurface_bezierCurveFillConstructOCCDecorator.h"
#include "aGXmlBuilder/bSplineCurve_pointInterpolateConstructOCCDecorator.h"
#include "aGXmlBuilder/bSplineCurve_pointConstructOCCDecorator.h"
#include "aGXmlBuilder/bSplineSurface_bSplineCurveFillConstructOCCDecorator.h"
#include "aGXmlBuilder/bSplineCurve_pointPickFromPartConstructOCCDecorator.h"
#include "aGXmlBuilder/bSplineCurve_pointConstructArcLengthParaOCCDecorator.h"
#include "aGXmlBuilder/bSplineCurve_straightExtentToSurfaceOCCDecorator.h"


#define __IFRET(className) \
    if ( strcmp(str, #className) == 0 ) { \
      return new className; \
    }
#define __IFRETCUSTOM(className, retClass) \
  if ( strcmp(str, #className) == 0 ) { \
    return new retClass; \
  }

namespace dtOO {
  dtXmlParserDecoratorFactory::dtXmlParserDecoratorFactory() {
  }

  dtXmlParserDecoratorFactory::~dtXmlParserDecoratorFactory() {
  }

  dtXmlParserDecorator * dtXmlParserDecoratorFactory::create(char const * const str) {
    dt__info(create(), << str <<  " creating ... ");
		
    __IFRET(baseContainerDecorator);   
		__IFRETCUSTOM(baseContainer, baseContainerDecorator);
    __IFRETCUSTOM(pointDefinition, baseContainerDecorator);
		__IFRETCUSTOM(vectorDefinition, baseContainerDecorator);
    __IFRET(analyticGeometryDecorator);   
    __IFRETCUSTOM(analyticGeometry, analyticGeometryDecorator);
    __IFRET(trans6SidedCubeDecorator);
    __IFRETCUSTOM(trans6SidedCube, trans6SidedCubeDecorator);
		__IFRET(bezierCurve_pointConstructOCCDecorator);
		__IFRETCUSTOM(
		  bezierCurve_pointConstructOCC, bezierCurve_pointConstructOCCDecorator
		);
		__IFRET(bSplineSurface_skinConstructOCCDecorator);
		__IFRETCUSTOM(
		  bSplineSurface_skinConstructOCC, bSplineSurface_skinConstructOCCDecorator
		);
		__IFRET(surfaceOfRevolution_curveRotateConstructOCCDecorator);
		__IFRETCUSTOM(
		  surfaceOfRevolution_curveRotateConstructOCC, 
			surfaceOfRevolution_curveRotateConstructOCCDecorator
		);
		__IFRET(bezierCurvePointPickFromPartConstructOCCDecorator);
		__IFRETCUSTOM(
		  bezierCurvePointPickFromPartConstructOCC, 
			bezierCurvePointPickFromPartConstructOCCDecorator
		);
    __IFRET(bSplineCurve_pointPickFromPartConstructOCCDecorator);
    __IFRETCUSTOM(
			bSplineCurve_pointPickFromPartConstructOCC, 
			bSplineCurve_pointPickFromPartConstructOCCDecorator
		);		
		__IFRET(bSplineCurve_curveConnectConstructOCCDecorator);
		__IFRETCUSTOM(
			bSplineCurve_curveConnectConstructOCC, 
			bSplineCurve_curveConnectConstructOCCDecorator
		);
		__IFRET(bezierSurface_bezierCurveFillConstructOCCDecorator);
		__IFRETCUSTOM(
			bezierSurface_bezierCurveFillConstructOCC, 
			bezierSurface_bezierCurveFillConstructOCCDecorator
		);
		__IFRET(bSplineCurve_pointInterpolateConstructOCCDecorator);
		__IFRETCUSTOM(
			bSplineCurve_pointInterpolateConstructOCC, 
			bSplineCurve_pointInterpolateConstructOCCDecorator
		);
		__IFRET(bSplineCurve_pointConstructOCCDecorator);
		__IFRETCUSTOM(
			bSplineCurve_pointConstructOCC, bSplineCurve_pointConstructOCCDecorator
		);
		__IFRET(bSplineSurface_bSplineCurveFillConstructOCCDecorator);
		__IFRETCUSTOM(
			bSplineSurface_bSplineCurveFillConstructOCC, 
			bSplineSurface_bSplineCurveFillConstructOCCDecorator
		);
		__IFRET(rotatingMap2dTo3dDecorator);
		__IFRETCUSTOM(rotatingMap2dTo3d, rotatingMap2dTo3dDecorator);
		__IFRET(bSplineCurve_pointPickFromPartConstructOCCDecorator);
		__IFRETCUSTOM(
			bSplineCurve_pointPickFromPartConstructOCC, 
			bSplineCurve_pointPickFromPartConstructOCCDecorator
		);
		__IFRET(bSplineCurve_pointConstructArcLengthParaOCCDecorator);
		__IFRETCUSTOM(
			bSplineCurve_pointConstructArcLengthParaOCC, 
			bSplineCurve_pointConstructArcLengthParaOCCDecorator
		);
		__IFRET(bSplineCurve_straightExtentToSurfaceOCCDecorator);
		__IFRETCUSTOM(
			bSplineCurve_straightExtentToSurfaceOCC, 
			bSplineCurve_straightExtentToSurfaceOCCDecorator
		);
		__IFRET(scaInMap1dTo3dDecorator);
		__IFRETCUSTOM(scaInMap1dTo3d, scaInMap1dTo3dDecorator);
    __IFRET( vec2dInMap2dTo3dDecorator );
		__IFRETCUSTOM( vec2dInMap2dTo3d, vec2dInMap2dTo3dDecorator );
		__IFRET(vec3dInMap3dTo3dDecorator);
		__IFRETCUSTOM(vec3dInMap3dTo3d, vec3dInMap3dTo3dDecorator);
		__IFRET(infinityMap3dTo3dDecorator);
		__IFRETCUSTOM(infinityMap3dTo3d, infinityMap3dTo3dDecorator);	
		__IFRET(makeCompoundDecorator);
		__IFRETCUSTOM(makeCompound, makeCompoundDecorator);
		__IFRET(vec3dInMap3dTo3dSurroundingInternalDecorator);
		__IFRETCUSTOM(
			vec3dInMap3dTo3dSurroundingInternal, 
			vec3dInMap3dTo3dSurroundingInternalDecorator
		);		
    __IFRET(map2dTo3dApproximateInMap3dTo3dDecorator);
    __IFRETCUSTOM(
      map2dTo3dApproximateInMap3dTo3d,
      map2dTo3dApproximateInMap3dTo3dDecorator
    );

    dt__throw(create(), <<  "Could not be created.");
  }

  dtXmlParserDecorator* dtXmlParserDecoratorFactory::create( std::string const str ) {
    return create( str.c_str() );
  }
}
