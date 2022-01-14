#ifndef VEC2DFUNCTION_H
#define	VEC2DFUNCTION_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include "analyticFunction.h"

namespace dtOO {
  class vec2dFunction : public analyticFunction {
  public:
    dt__class(vec2dFunction, analyticFunction);     
    vec2dFunction();
    vec2dFunction(const vec2dFunction& orig);
    virtual ~vec2dFunction();
    virtual int yDim( void ) const;    
    dtPoint2 YdtPoint2(aFX const & xx) const;
    virtual bool closed( int const & dir ) const = 0;    
  };
}
#endif	/* VEC2DFUNCTION_H */

