#ifndef VEC3DFUNCTION_H
#define	VEC3DFUNCTION_H

#include <dtOOTypeDef.h>

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
    virtual dtInt yDim( void ) const;    
    dtPoint3 YdtPoint3(aFX const & xx) const;
    virtual bool closed( dtInt const & dir ) const = 0;
  };
}
#endif	/* VEC3DFUNCTION_H */

