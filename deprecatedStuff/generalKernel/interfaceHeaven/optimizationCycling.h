#ifndef OPTIMIZATIONCYCLING_H
#define	OPTIMIZATIONCYCLING_H

#include <logMe/dtMacros.h>

namespace dtOO {
  class optimizationCycling {
  public:
    dt__classOnlyName(optimizationCycling);
    optimizationCycling();
    optimizationCycling(const optimizationCycling& orig);
    virtual ~optimizationCycling();
    virtual void doCycle( void ) = 0;
  private:

  };
}
#endif	/* OPTIMIZATIONCYCLING_H */

