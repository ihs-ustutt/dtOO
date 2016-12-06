#include "threadSafeRoot.h"

#include <logMe/logMe.h>
#include <interfaceHeaven/staticPropertiesHandler.h>
#include <omp.h>
#include <TThread.h>
#include <TError.h>

namespace dtOO {
  threadSafeRoot::threadSafeRoot() {
    if ( omp_in_parallel() ) {
      _thread.reset( new ::TThread() );
      if ( 
        staticPropertiesHandler::getInstance()->getOptionInt("root_printLevel") 
        ==
        0
      ) {
        gErrorIgnoreLevel = 1001;
      }
    }
  }

  threadSafeRoot::~threadSafeRoot() {
  }
}

