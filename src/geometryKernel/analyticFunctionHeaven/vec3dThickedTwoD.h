/*---------------------------------------------------------------------------*\
  dtOO < design tool Object-Oriented >

    Copyright (C) 2024 A. Tismer.
-------------------------------------------------------------------------------
License
    This file is part of dtOO.

    dtOO is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the LICENSE.txt file in the
    dtOO root directory for more details.

    You should have received a copy of the License along with dtOO.

\*---------------------------------------------------------------------------*/

#ifndef VEC3DTHICKEDTWOD_H
#define VEC3DTHICKEDTWOD_H

#include <dtOOTypeDef.h>

#include "vec3dTwoD.h"
#include <dtLinearAlgebra.h>
#include <interfaceHeaven/ptrHandling.h>
#include <logMe/dtMacros.h>

namespace dtOO {
class dtTransformer;

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
  vec3dThickedTwoD(const vec3dThickedTwoD &orig);
  vec3dThickedTwoD(
    vec3dTwoD const *const ff, vec3dTwoD const *const tt, dtVector3 const nf
  );
  virtual vec3dThickedTwoD *clone(void) const;
  virtual vec3dThickedTwoD *cloneTransformed(dtTransformer const *const dtT
  ) const;
  virtual vec3dThickedTwoD *create(void) const;
  virtual ~vec3dThickedTwoD();
  virtual aFY Y(aFX const &xx) const;
  virtual bool closed(dtInt const &dir) const;

private:
  dt__pH(vec3dTwoD) _ff;
  dt__pH(vec3dTwoD) _tt;
  //    dt__pH(scaTwoD) _s2d1;
  dtVector3 _nf;
};
} // namespace dtOO
#endif /* VEC3DTHICKEDTWOD_H */
