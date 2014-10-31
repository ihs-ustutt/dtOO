#ifndef VEC2DFUNCTION_H
#define	VEC2DFUNCTION_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include "analyticFunction.h"

namespace dtOO {
  class vec2dFunction : public analyticFunction {
  public:
    dt__CLASSSTD(vec2dFunction, analyticFunction);     
    vec2dFunction();
    vec2dFunction(const vec2dFunction& orig);
    virtual ~vec2dFunction();
    dtPoint2 YdtPoint2(aFX const & xx) const;
  private:

  };
}
#endif	/* VEC2DFUNCTION_H */

