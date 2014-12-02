#include "dtXmlParserDecoratorFactory.h"

#include "dtXmlParserDecorator.h"
#include <logMe/logMe.h>
#include <string.h>
#include "generalBuilder/baseContainerDecorator.h"
#include "generalBuilder/analyticGeometryDecorator.h"
#include "generalBuilder/trans6SidedCubeDecorator.h"
#include "generalBuilder/vec3dInMap3dTo3dDecorator.h"
#include "generalBuilder/rotatingMap2dTo3dDecorator.h"
#include "generalBuilder/infinityMap3dTo3dDecorator.h"
#include "occBuilder/bezierCurve_pointConstructOCCDecorator.h"
#include "occBuilder/bSplineCurve_curveConnectConstructOCCDecorator.h"
#include "occBuilder/bSplineSurface_skinConstructOCCDecorator.h"
#include "occBuilder/surfaceOfRevolution_curveRotateConstructOCCDecorator.h"
#include "occBuilder/bezierCurvePointPickFromPartConstructOCCDecorator.h"
#include "occBuilder/bSplineCurve_pointPickFromPartConstructOCCDecorator.h"
#include "occBuilder/bezierSurface_bezierCurveFillConstructOCCDecorator.h"
#include "occBuilder/bSplineCurve_pointInterpolateConstructOCCDecorator.h"
#include "occBuilder/bSplineCurve_pointConstructOCCDecorator.h"
#include "occBuilder/bSplineSurface_bSplineCurveFillConstructOCCDecorator.h"
#include "occBuilder/bSplineCurve_pointPickFromPartConstructOCCDecorator.h"
#include "occBuilder/bSplineCurve_pointConstructArcLengthParaOCCDecorator.h"


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
    DTINFOWF(create(), << str <<  " creating ... ");
		
    __IFRET(baseContainerDecorator);   
		__IFRETCUSTOM(baseContainer, baseContainerDecorator);
    __IFRETCUSTOM(pointDefinition, baseContainerDecorator);
		__IFRETCUSTOM(vectorDefinition, baseContainerDecorator);
    __IFRET(analyticGeometryDecorator);   
    __IFRETCUSTOM(analyticGeometry, analyticGeometryDecorator);
    __IFRET(trans6SidedCubeDecorator);
    __IFRETCUSTOM(trans6SidedCube, trans6SidedCubeDecorator);
		__IFRET(bezierCurve_pointConstructOCCDecorator);
		__IFRETCUSTOM(bezierCurve_pointConstructOCC, bezierCurve_pointConstructOCCDecorator);
		__IFRET(bSplineSurface_skinConstructOCCDecorator);
		__IFRETCUSTOM(bSplineSurface_skinConstructOCC, bSplineSurface_skinConstructOCCDecorator);
		__IFRET(surfaceOfRevolution_curveRotateConstructOCCDecorator);
		__IFRETCUSTOM(surfaceOfRevolution_curveRotateConstructOCC, surfaceOfRevolution_curveRotateConstructOCCDecorator);
		__IFRET(bezierCurvePointPickFromPartConstructOCCDecorator);
		__IFRETCUSTOM(bezierCurvePointPickFromPartConstructOCC, bezierCurvePointPickFromPartConstructOCCDecorator);
    __IFRET(bSplineCurve_pointPickFromPartConstructOCCDecorator);
    __IFRETCUSTOM(bSplineCurve_pointPickFromPartConstructOCC, bSplineCurve_pointPickFromPartConstructOCCDecorator);		
		__IFRET(bSplineCurve_curveConnectConstructOCCDecorator);
		__IFRETCUSTOM(bSplineCurve_curveConnectConstructOCC, bSplineCurve_curveConnectConstructOCCDecorator);
		__IFRET(bezierSurface_bezierCurveFillConstructOCCDecorator);
		__IFRETCUSTOM(bezierSurface_bezierCurveFillConstructOCC, bezierSurface_bezierCurveFillConstructOCCDecorator);
		__IFRET(bSplineCurve_pointInterpolateConstructOCCDecorator);
		__IFRETCUSTOM(bSplineCurve_pointInterpolateConstructOCC, bSplineCurve_pointInterpolateConstructOCCDecorator);
		__IFRET(bSplineCurve_pointConstructOCCDecorator);
		__IFRETCUSTOM(bSplineCurve_pointConstructOCC, bSplineCurve_pointConstructOCCDecorator);
		__IFRET(bSplineSurface_bSplineCurveFillConstructOCCDecorator);
		__IFRETCUSTOM(bSplineSurface_bSplineCurveFillConstructOCC, bSplineSurface_bSplineCurveFillConstructOCCDecorator);
		__IFRET(rotatingMap2dTo3dDecorator);
		__IFRETCUSTOM(rotatingMap2dTo3d, rotatingMap2dTo3dDecorator);
		__IFRET(bSplineCurve_pointPickFromPartConstructOCCDecorator);
		__IFRETCUSTOM(bSplineCurve_pointPickFromPartConstructOCC, bSplineCurve_pointPickFromPartConstructOCCDecorator);
		__IFRET(bSplineCurve_pointConstructArcLengthParaOCCDecorator);
		__IFRETCUSTOM(bSplineCurve_pointConstructArcLengthParaOCC, bSplineCurve_pointConstructArcLengthParaOCCDecorator);
		__IFRET(vec3dInMap3dTo3dDecorator);
		__IFRETCUSTOM(vec3dInMap3dTo3d, vec3dInMap3dTo3dDecorator);
		__IFRET(infinityMap3dTo3dDecorator);
		__IFRETCUSTOM(infinityMap3dTo3d, infinityMap3dTo3dDecorator);		
		
    dt__THROW(create(), <<  "Could not be created.");
  }

  dtXmlParserDecorator* dtXmlParserDecoratorFactory::create(string const str) {
    return create( str.c_str() );
  }
}
