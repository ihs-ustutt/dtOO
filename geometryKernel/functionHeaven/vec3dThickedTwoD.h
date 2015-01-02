#ifndef VEC3DTHICKEDTWOD_H
#define	VEC3DTHICKEDTWOD_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include "vec3dTwoD.h"
#include <interfaceHeaven/ptrHandling.h>

namespace dtOO {
  class scaTwoD;
  
  class vec3dThickedTwoD : public vec3dTwoD {
  public:
    dt__CLASSSTD(vec3dThickedTwoD, analyticFunction);     
    vec3dThickedTwoD();
    vec3dThickedTwoD(const vec3dThickedTwoD& orig);
    vec3dThickedTwoD(vec3dTwoD const * const v3d2d, scaTwoD const * const s2d);
    virtual vec3dThickedTwoD * clone( void ) const;
    virtual vec3dThickedTwoD * create( void ) const;    
    virtual ~vec3dThickedTwoD();
    virtual aFY Y( aFX const & xx) const;
    virtual bool closed( int const & dir ) const;
  private:
    dt__pH(vec3dTwoD) _v3d2d;
    dt__pH(scaTwoD) _s2d;
  };
}
#endif	/* VEC3DTHICKEDTWOD_H */

