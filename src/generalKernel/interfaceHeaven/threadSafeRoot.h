#ifndef THREADSAFEROOT_H
#define	THREADSAFEROOT_H

#include <dtOOTypeDef.h>

#include <logMe/dtMacros.h>

class TThread;

namespace dtOO {
  class threadSafeRoot {
  public:
    dt__classOnlyName(threadSafeRoot);    
    threadSafeRoot();
    virtual ~threadSafeRoot();
  private:
    dt__pH(::TThread) _thread;
  };
}
#endif	/* THREADSAFEROOT_H */

