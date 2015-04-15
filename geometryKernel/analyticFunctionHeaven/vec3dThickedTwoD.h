#ifndef VEC3DTHICKEDTWOD_H
#define	VEC3DTHICKEDTWOD_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include "vec3dTwoD.h"
#include <interfaceHeaven/ptrHandling.h>

namespace dtOO { 
  /**
   * Add a scalar function \f$s(\underline{x})\f$ value to a vector
   * function \f$\underline{f}(u,v)\f$ according to
   * 
   * \f[
   *   \underline{t}(u,v) 
   *   = 
   *   \underline{f}(u,v)
   *   +
   *   s(\underline{x})
   *   \left(
   *     \underline{n}_f(u,v) 
   *     - 
   *     \left(
   *       \underline{n}_f(u,v) \cdot \underline{c}
   *     \right) 
   *     \cdot 
   *     \underline{c}
   *   \right) \text{.}
   * \f]
   * $\underline{c} is user given vector.
   */
  class vec3dThickedTwoD : public vec3dTwoD {
  public:
    dt__class(vec3dThickedTwoD, analyticFunction);     
    vec3dThickedTwoD();
    vec3dThickedTwoD(const vec3dThickedTwoD& orig);
    vec3dThickedTwoD(
      vec3dTwoD const * const ff, 
      vec3dTwoD const * const tt,
      dtVector3 const nf
    );
    virtual vec3dThickedTwoD * clone( void ) const;
    virtual vec3dThickedTwoD * create( void ) const;    
    virtual ~vec3dThickedTwoD();
    virtual aFY Y( aFX const & xx) const;
    virtual bool closed( int const & dir ) const;
  private:
    dt__pH(vec3dTwoD) _ff;
    dt__pH(vec3dTwoD) _tt;
//    dt__pH(scaTwoD) _s2d1;
    dtVector3 _nf;
  };
}
#endif	/* VEC3DTHICKEDTWOD_H */

