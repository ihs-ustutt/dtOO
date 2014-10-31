#include "constValueReconstructor.h"
#include "constValue.h"

#include <do/coDoSet.h>

namespace dtOO {
  constValueReconstructor::constValueReconstructor() {
  }

  constValueReconstructor::~constValueReconstructor() {
  }
  
  constValue * constValueReconstructor::reconFromCoDoSet ( 
                 covise::coDoSet const * const obj 
               ) const {
    
    constValue * ret = constValueFactory::create( obj->getAttribute("factoryType") );
    
    ret->unpackFromCoDoSet(obj);
    
    return ret;
  }
}
