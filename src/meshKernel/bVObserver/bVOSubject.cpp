#include "bVOSubject.h"

#include <logMe/logMe.h>
#include <logMe/logContainer.h>
#include "bVOInterface.h"
#include <boundedVolume.h>

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
    dt__forAllRefAuto(_observers, anObserver) {
      if ( ! anObserver->constRefBoundedVolume().isMeshed() ) {
        anObserver->preUpdate();
      }
    }
  }
	
  void bVOSubject::postNotify( void ) {
    dt__forAllRefAuto(_observers, anObserver) anObserver->postUpdate();
  }	

  void bVOSubject::dump( void ) const {
    logContainer< bVOSubject > logC(TLogLevel::logINFO, "dump()");
    int cc = 0;
    dt__forAllRefAuto(_observers, anOb) {
      logC() << "[ " << cc << " ] : " << anOb->virtualClassName() << std::endl;
      cc++;
    }
  }
}
