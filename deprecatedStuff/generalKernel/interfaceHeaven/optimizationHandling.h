#ifndef OPTIMIZATIONHANDLING_H
#define	OPTIMIZATIONHANDLING_H

#include <logMe/dtMacros.h>

namespace dtOO {
  class optimizationHandling {
  public:
    dt__CLASSNAME(optimizationHandling);
    optimizationHandling();
    optimizationHandling(const optimizationHandling& orig);
    virtual ~optimizationHandling();
    virtual float characterizeMe( void ) = 0;
    virtual float characterizeFailedDesign( void ) = 0;
  private:

  };
}

#endif	/* OPTIMIZATIONHANDLING_H */

