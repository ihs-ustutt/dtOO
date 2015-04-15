#ifndef VEC3DFUNCTION_H
#define	VEC3DFUNCTION_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include "analyticFunction.h"

namespace dtOO {
  class vec3dFunction : public analyticFunction {
  public:
    dt__class(vec3dFunction, analyticFunction);     
    vec3dFunction();
    vec3dFunction(const vec3dFunction& orig);
    virtual ~vec3dFunction();
    dtPoint3 YdtPoint3(aFX const & xx) const;
    virtual bool closed( int const & dir ) const = 0;
  private:

  };
}
#endif	/* VEC3DFUNCTION_H */

