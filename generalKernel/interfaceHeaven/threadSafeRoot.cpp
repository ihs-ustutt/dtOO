#include "threadSafeRoot.h"

#include <logMe/logMe.h>
#include <omp.h>
#include <TThread.h>

namespace dtOO {
  threadSafeRoot::threadSafeRoot() {
    if (omp_in_parallel() ) _thread.reset( new ::TThread() );
  }

  threadSafeRoot::~threadSafeRoot() {
  }
}

