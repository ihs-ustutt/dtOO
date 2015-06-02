#ifndef VEC3DSURFACETWOD_H
#define	VEC3DSURFACETWOD_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include "vec3dTwoD.h"
#include <interfaceHeaven/ptrHandling.h>
#include "analyticFunctionCompound.h"

namespace dtOO {
  class dtSurface;
  
  class vec3dSurfaceTwoD : public vec3dTwoD {
  public:
    dt__class(vec3dSurfaceTwoD, analyticFunction);     
    vec3dSurfaceTwoD();
    vec3dSurfaceTwoD(const vec3dSurfaceTwoD& orig);
    vec3dSurfaceTwoD(dtSurface const * const dtS);
    virtual vec3dSurfaceTwoD * clone( void ) const;
    virtual vec3dSurfaceTwoD * create( void ) const;    
    virtual ~vec3dSurfaceTwoD();
    virtual aFY Y( aFX const & xx) const;
    dtSurface const * ptrDtSurface( void ) const;
    virtual bool closed( int const & dir ) const;
    virtual aFX invY(aFY const & yy) const;    
  private:
    ptrHandling<dtSurface> _dtS;
  };
  typedef analyticFunctionCompound<vec3dSurfaceTwoD> vec3dSurfaceTwoDCompound;
}
#endif	/* VEC3DSURFACETWOD_H */

