#include "bVOSubject.h"

#include <logMe/logMe.h>
#include "bVOInterface.h"

namespace dtOO {
  bVOSubject::bVOSubject() {

  }

  bVOSubject::~bVOSubject() {
    _observers.destroy();
  }

  void bVOSubject::attachBVObserver( bVOInterface * observer ) {
    _observers.push_back( observer );
  }

  void bVOSubject::preNotify( void ) {
    dt__forAllIndex(_observers, ii) _observers[ii]->preUpdate();
  }
	
  void bVOSubject::postNotify( void ) {
    dt__forAllIndex(_observers, ii) _observers[ii]->postUpdate();
  }	
}