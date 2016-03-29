#include "aGXmlBuilderFactory.h"

#include "aGXmlBuilder.h"
#include <logMe/logMe.h>
#include <string.h>
#include "aGXmlBuilder/baseContainerAGXmlBuilder.h"
#include "aGXmlBuilder/analyticGeometryAGXmlBuilder.h"
#include "aGXmlBuilder/trans6SidedCubeAGXmlBuilder.h"
#include "aGXmlBuilder/scaInMap1dTo3dAGXmlBuilder.h"
#include "aGXmlBuilder/vec2dInMap2dTo3dAGXmlBuilder.h"
#include "aGXmlBuilder/vec3dInMap3dTo3dAGXmlBuilder.h"
#include "aGXmlBuilder/rotatingMap2dTo3dAGXmlBuilder.h"
#include "aGXmlBuilder/infinityMap3dTo3dAGXmlBuilder.h"
#include "aGXmlBuilder/makeCompoundAGXmlBuilder.h"
#include "aGXmlBuilder/vec3dInMap3dTo3dSurroundingInternalAGXmlBuilder.h"
#include "aGXmlBuilder/map2dTo3dApproximateInMap3dTo3dAGXmlBuilder.h"
#include "aGXmlBuilder/bezierCurve_pointConstructOCCAGXmlBuilder.h"
#include "aGXmlBuilder/bSplineCurve_curveConnectConstructOCCAGXmlBuilder.h"
#include "aGXmlBuilder/bSplineSurface_skinConstructOCCAGXmlBuilder.h"
#include "aGXmlBuilder/surfaceOfRevolution_curveRotateConstructOCCAGXmlBuilder.h"
#include "aGXmlBuilder/bezierCurvePointPickFromPartConstructOCCAGXmlBuilder.h"
#include "aGXmlBuilder/bSplineCurve_pointPickFromPartConstructOCCAGXmlBuilder.h"
#include "aGXmlBuilder/bezierSurface_bezierCurveFillConstructOCCAGXmlBuilder.h"
#include "aGXmlBuilder/bSplineCurve_pointInterpolateConstructOCCAGXmlBuilder.h"
#include "aGXmlBuilder/bSplineCurve_pointConstructOCCAGXmlBuilder.h"
#include "aGXmlBuilder/bSplineSurface_bSplineCurveFillConstructOCCAGXmlBuilder.h"
#include "aGXmlBuilder/bSplineCurve_pointPickFromPartConstructOCCAGXmlBuilder.h"
#include "aGXmlBuilder/bSplineCurve_pointConstructArcLengthParaOCCAGXmlBuilder.h"
#include "aGXmlBuilder/bSplineCurve_straightExtentToSurfaceOCCAGXmlBuilder.h"
#include "aGXmlBuilder/vec2dOneDInMap2dTo3d_closestCurveAGXmlBuilder.h"
#include "aGXmlBuilder/geomSurface_readStepOCCAGXmlBuilder.h"
#include "aGXmlBuilder/geomSurface_readStlOCCAGXmlBuilder.h"
#include "aGXmlBuilder/geomShape_readIgesOCCAGXmlBuilder.h"

#define __IFRET(className) \
    if ( strcmp(str, #className) == 0 ) { \
      return new className; \
    }
#define __IFRETCUSTOM(className, retClass) \
  if ( strcmp(str, #className) == 0 ) { \
    return new retClass; \
  }

namespace dtOO {
  aGXmlBuilderFactory::aGXmlBuilderFactory() {
  }

  aGXmlBuilderFactory::~aGXmlBuilderFactory() {
  }

  aGXmlBuilder * aGXmlBuilderFactory::create(char const * const str) {
    dt__info(create(), << str <<  " creating ... ");
		
    __IFRET(baseContainerAGXmlBuilder);   
		__IFRETCUSTOM(baseContainer, baseContainerAGXmlBuilder);
    __IFRETCUSTOM(pointDefinition, baseContainerAGXmlBuilder);
		__IFRETCUSTOM(vectorDefinition, baseContainerAGXmlBuilder);
    __IFRET(analyticGeometryAGXmlBuilder);   
    __IFRETCUSTOM(analyticGeometry, analyticGeometryAGXmlBuilder);
    __IFRET(trans6SidedCubeAGXmlBuilder);
    __IFRETCUSTOM(trans6SidedCube, trans6SidedCubeAGXmlBuilder);
		__IFRET(bezierCurve_pointConstructOCCAGXmlBuilder);
		__IFRETCUSTOM(
		  bezierCurve_pointConstructOCC, bezierCurve_pointConstructOCCAGXmlBuilder
		);
		__IFRET(bSplineSurface_skinConstructOCCAGXmlBuilder);
		__IFRETCUSTOM(
		  bSplineSurface_skinConstructOCC, bSplineSurface_skinConstructOCCAGXmlBuilder
		);
		__IFRET(surfaceOfRevolution_curveRotateConstructOCCAGXmlBuilder);
		__IFRETCUSTOM(
		  surfaceOfRevolution_curveRotateConstructOCC, 
			surfaceOfRevolution_curveRotateConstructOCCAGXmlBuilder
		);
		__IFRET(bezierCurvePointPickFromPartConstructOCCAGXmlBuilder);
		__IFRETCUSTOM(
		  bezierCurvePointPickFromPartConstructOCC, 
			bezierCurvePointPickFromPartConstructOCCAGXmlBuilder
		);
    __IFRET(bSplineCurve_pointPickFromPartConstructOCCAGXmlBuilder);
    __IFRETCUSTOM(
			bSplineCurve_pointPickFromPartConstructOCC, 
			bSplineCurve_pointPickFromPartConstructOCCAGXmlBuilder
		);		
		__IFRET(bSplineCurve_curveConnectConstructOCCAGXmlBuilder);
		__IFRETCUSTOM(
			bSplineCurve_curveConnectConstructOCC, 
			bSplineCurve_curveConnectConstructOCCAGXmlBuilder
		);
		__IFRET(bezierSurface_bezierCurveFillConstructOCCAGXmlBuilder);
		__IFRETCUSTOM(
			bezierSurface_bezierCurveFillConstructOCC, 
			bezierSurface_bezierCurveFillConstructOCCAGXmlBuilder
		);
		__IFRET(bSplineCurve_pointInterpolateConstructOCCAGXmlBuilder);
		__IFRETCUSTOM(
			bSplineCurve_pointInterpolateConstructOCC, 
			bSplineCurve_pointInterpolateConstructOCCAGXmlBuilder
		);
		__IFRET(bSplineCurve_pointConstructOCCAGXmlBuilder);
		__IFRETCUSTOM(
			bSplineCurve_pointConstructOCC, bSplineCurve_pointConstructOCCAGXmlBuilder
		);
		__IFRET(bSplineSurface_bSplineCurveFillConstructOCCAGXmlBuilder);
		__IFRETCUSTOM(
			bSplineSurface_bSplineCurveFillConstructOCC, 
			bSplineSurface_bSplineCurveFillConstructOCCAGXmlBuilder
		);
		__IFRET(rotatingMap2dTo3dAGXmlBuilder);
		__IFRETCUSTOM(rotatingMap2dTo3d, rotatingMap2dTo3dAGXmlBuilder);
		__IFRET(bSplineCurve_pointPickFromPartConstructOCCAGXmlBuilder);
		__IFRETCUSTOM(
			bSplineCurve_pointPickFromPartConstructOCC, 
			bSplineCurve_pointPickFromPartConstructOCCAGXmlBuilder
		);
		__IFRET(bSplineCurve_pointConstructArcLengthParaOCCAGXmlBuilder);
		__IFRETCUSTOM(
			bSplineCurve_pointConstructArcLengthParaOCC, 
			bSplineCurve_pointConstructArcLengthParaOCCAGXmlBuilder
		);
		__IFRET(bSplineCurve_straightExtentToSurfaceOCCAGXmlBuilder);
		__IFRETCUSTOM(
			bSplineCurve_straightExtentToSurfaceOCC, 
			bSplineCurve_straightExtentToSurfaceOCCAGXmlBuilder
		);
		__IFRET(scaInMap1dTo3dAGXmlBuilder);
		__IFRETCUSTOM(scaInMap1dTo3d, scaInMap1dTo3dAGXmlBuilder);
    __IFRET( vec2dInMap2dTo3dAGXmlBuilder );
		__IFRETCUSTOM( vec2dInMap2dTo3d, vec2dInMap2dTo3dAGXmlBuilder );
		__IFRET(vec3dInMap3dTo3dAGXmlBuilder);
		__IFRETCUSTOM(vec3dInMap3dTo3d, vec3dInMap3dTo3dAGXmlBuilder);
		__IFRET(infinityMap3dTo3dAGXmlBuilder);
		__IFRETCUSTOM(infinityMap3dTo3d, infinityMap3dTo3dAGXmlBuilder);	
		__IFRET(makeCompoundAGXmlBuilder);
		__IFRETCUSTOM(makeCompound, makeCompoundAGXmlBuilder);
		__IFRET(vec3dInMap3dTo3dSurroundingInternalAGXmlBuilder);
		__IFRETCUSTOM(
			vec3dInMap3dTo3dSurroundingInternal, 
			vec3dInMap3dTo3dSurroundingInternalAGXmlBuilder
		);		
    __IFRET(map2dTo3dApproximateInMap3dTo3dAGXmlBuilder);
    __IFRETCUSTOM(
      map2dTo3dApproximateInMap3dTo3d,
      map2dTo3dApproximateInMap3dTo3dAGXmlBuilder
    );
    __IFRET(vec2dOneDInMap2dTo3d_closestCurveAGXmlBuilder);
    __IFRETCUSTOM(
      vec2dOneDInMap2dTo3d_closestCurve,
      vec2dOneDInMap2dTo3d_closestCurveAGXmlBuilder
    );
    __IFRET(geomSurface_readStepOCCAGXmlBuilder);
    __IFRETCUSTOM(geomSurface_readStepOCC, geomSurface_readStepOCCAGXmlBuilder);
    __IFRET(geomSurface_readStlOCCAGXmlBuilder);
    __IFRETCUSTOM(geomSurface_readStlOCC, geomSurface_readStlOCCAGXmlBuilder);    
    __IFRET(geomShape_readIgesOCCAGXmlBuilder);
    __IFRETCUSTOM(geomShape_readIgesOCC, geomShape_readIgesOCCAGXmlBuilder);
    
    dt__throw(
      create(), 
      << str <<  " could not be created." << std::endl
      << "> -------------------------------------------------------" << std::endl
      << "> Please note: ""Decorator"" at the end of the" << std::endl
      << "> builder name is no longer supported. Please erase it" << std::endl
      << "> or replace it by ""AGXmlBuilder""." << std::endl
      << "> -------------------------------------------------------"
      );
  }

  aGXmlBuilder* aGXmlBuilderFactory::create( std::string const str ) {
    return create( str.c_str() );
  }
}
