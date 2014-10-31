#ifndef CONSTVALUERECONSTRUCTOR_H
#define	CONSTVALUERECONSTRUCTOR_H

#include "constValueFactory.h"

namespace covise {
  class coDoSet;
}

namespace dtOO {
  class constValueReconstructor : public constValueFactory {
  public:
    constValueReconstructor();
    virtual ~constValueReconstructor();
    constValue * reconFromCoDoSet( covise::coDoSet const * const obj ) const;
  private:

  };
}
#endif	/* CONSTVALUERECONSTRUCTOR_H */

