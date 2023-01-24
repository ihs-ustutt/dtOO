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

  void lVHOSubject::letObserve() {
    dt__debug(letObserve(), << "Call");
  }
}
