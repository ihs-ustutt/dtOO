#ifndef VEC2DSURFACE2DTWOD_H
#define	VEC2DSURFACE2DTWOD_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include "vec2dTwoD.h"
#include <interfaceHeaven/ptrHandling.h>

namespace dtOO {
  class dtSurface2d;
  class dtTransformer;  
  
  class vec2dSurface2dTwoD : public vec2dTwoD {
  public:
    dt__class(vec2dSurface2dTwoD, analyticFunction);     
    vec2dSurface2dTwoD();
    vec2dSurface2dTwoD(const vec2dSurface2dTwoD& orig);
    vec2dSurface2dTwoD(dtSurface2d const * const dtS2d);
    virtual vec2dSurface2dTwoD * clone( void ) const;
    virtual vec2dSurface2dTwoD * cloneTransformed(
      dtTransformer const * const dtT 
    ) const;          
    virtual vec2dSurface2dTwoD * create( void ) const;    
    virtual ~vec2dSurface2dTwoD();
    virtual aFY Y( aFX const & xx) const;
    dtSurface2d const * ptrDtSurface2d( void ) const;
    virtual bool closed( int const & dir ) const;
  private:
    ptrHandling<dtSurface2d> _dtS2d;
  };
}
#endif	/* VEC2DSURFACE2DTWOD_H */

