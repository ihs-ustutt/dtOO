#ifndef SCAFUNCTIONRECONSTRUCTOR_H
#define	SCAFUNCTIONRECONSTRUCTOR_H

#include "scaFunctionFactory.h"

namespace covise {
  class coDoSet;
}

namespace dtOO {
  class scaFunctionReconstructor : public scaFunctionFactory {
  public:
    scaFunctionReconstructor();
    virtual ~scaFunctionReconstructor();
    scaFunction * reconFromCoDoSet( covise::coDoSet const * const obj ) const;
  private:

  };
}
#endif	/* SCAFUNCTIONRECONSTRUCTOR_H */

