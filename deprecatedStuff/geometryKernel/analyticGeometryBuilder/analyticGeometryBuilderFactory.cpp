#include "analyticGeometryBuilderFactory.h"
#include <logMe/logMe.h>
#include "analyticGeometryBuilder.h"
//#include "rotSplineBuilder.h"
//#include "spline3dBuilder.h"
//#include "spline3dToSurfaceBuilder.h"
//#include "spline3dToBlendedSurfaceBuilder.h"

#define __IFRET(className) \
    if ( strcmp(str, #className) == 0 ) { \
      return new className; \
    }
//#define __IFRETCUSTOM(className, retClass) \
//  if ( strcmp(str, #className) == 0 ) { \
//    return new retClass; \
//  }
namespace dtOO {
  DTCLASSLOGMETHODIN(analyticGeometryBuilderFactory);

  analyticGeometryBuilderFactory::analyticGeometryBuilderFactory() {
  }

  analyticGeometryBuilderFactory::~analyticGeometryBuilderFactory() {
  }

  analyticGeometryBuilder* analyticGeometryBuilderFactory::create(char const * const str) const {
    DTINFOWF(create(), << str <<  " creating ... ");

//    __IFRET(rotSplineBuilder);    
//    __IFRET(spline3dBuilder);   
//    __IFRET(spline3dToSurfaceBuilder);   
//    __IFRET(spline3dToBlendedSurfaceBuilder);    

    dt__THROW(create(), <<  "Could not be created.");
  }
}

