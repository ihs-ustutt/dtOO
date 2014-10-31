#include "analyticGeometryReconstructor.h"
#include "analyticGeometry.h"

#include <do/coDoSet.h>

namespace dtOO {
  analyticGeometryReconstructor::analyticGeometryReconstructor() {
  }

  analyticGeometryReconstructor::~analyticGeometryReconstructor() {
  }
  
  analyticGeometry * analyticGeometryReconstructor::reconFromCoDoSet ( 
                 covise::coDoSet const * const obj 
               ) const {
    
    analyticGeometry * ret = analyticGeometryFactory::create( obj->getAttribute("factoryType") );
    
    ret->unpackFromCoDoSet(obj);
    
    return ret;
  }  
}
