#include "analyticGeometryFactory.h"
#include <logMe/logMe.h>
#include "analyticGeometry.h"
#include "splineCurve3d.h"
#include "analyticRotatingMap1dTo3d.h"
#include "analyticSurface.h"
#include "trans6SidedCube.h"
#include "rotatingMap2dTo3d.h"
#include "scaOneDInMap1dTo3d.h"
#include "vec3dTwoDInMap3dTo3d.h"
#include "vec3dOneDInMap3dTo3d.h"
#include "vec2dOneDInMap2dTo3d.h"
#include "vec2dTwoDInMap2dTo3d.h"
#include "infinityMap3dTo3d.h"

#define __IFRET(className) \
    if ( strcmp(str, #className) == 0 ) { \
      return new className; \
    }
#define __IFRETCUSTOM(className, retClass) \
  if ( strcmp(str, #className) == 0 ) { \
    return new retClass; \
  }
namespace dtOO {
  analyticGeometryFactory::analyticGeometryFactory() {
  }

  analyticGeometryFactory::~analyticGeometryFactory() {
  }

  analyticGeometry * analyticGeometryFactory::create(char const * const str) {
    dt__info(create(), << str <<  " creating ... ");
    
    __IFRET(splineCurve3d);
    __IFRET(analyticSurface);
    __IFRET(analyticRotatingMap1dTo3d);
    __IFRET(trans6SidedCube);
		__IFRET(rotatingMap2dTo3d);
		__IFRET(scaOneDInMap1dTo3d);
		__IFRET(vec3dTwoDInMap3dTo3d);
		__IFRET(vec3dOneDInMap3dTo3d);
		__IFRET(vec2dOneDInMap2dTo3d);
		__IFRET(vec2dTwoDInMap2dTo3d);
		__IFRET(infinityMap3dTo3d);
        
    dt__throw(create(), <<  "Could not be created.");
  }
}