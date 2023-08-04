#include "lVHOSubject.h"

#include <logMe/logMe.h>

namespace dtOO {  
  lVHOSubject::lVHOSubject() {
  }
  
  lVHOSubject::~lVHOSubject() {
  }

  void lVHOSubject::attach(lVHOInterface * observer) {
    dt__debug(attach(), << "Call");
  }

  void lVHOSubject::letObserve( lVHOInterface * callerToObserve ) {
    dt__debug(letObserve(), << "Call");
  }

  std::vector< labelHandling * > lVHOSubject::internalStdVector( void ) {
    dt__throw(internalStdVector(), << "Call");
  }

  dt__pVH(lVHOInterface) const & lVHOSubject::refObserver( void ) const {
    dt__throw(refObserver(), << "Call");
  }
}
