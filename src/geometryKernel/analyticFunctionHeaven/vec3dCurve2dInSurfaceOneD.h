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

#ifndef VEC3DCURVE2DINSURFACEONED_H
#define VEC3DCURVE2DINSURFACEONED_H

#include <dtOOTypeDef.h>

#include "vec3dOneD.h"
#include <dtLinearAlgebra.h>
#include <interfaceHeaven/ptrHandling.h>
#include <interfaceHeaven/vectorHandling.h>
#include <logMe/dtMacros.h>

namespace dtOO {
class dtCurve2d;
class dtSurface;
class renderInterface;
class dtTransformer;

class vec3dCurve2dInSurfaceOneD : public vec3dOneD {
public:
  dt__class(vec3dCurve2dInSurfaceOneD, analyticFunction);
  vec3dCurve2dInSurfaceOneD();
  vec3dCurve2dInSurfaceOneD(const vec3dCurve2dInSurfaceOneD &orig);
  vec3dCurve2dInSurfaceOneD(
    dtCurve2d const *const dtC2d, dtSurface const *const dtS
  );
  virtual vec3dCurve2dInSurfaceOneD *clone(void) const;
  virtual vec3dCurve2dInSurfaceOneD *
  cloneTransformed(dtTransformer const *const dtT) const;
  virtual vec3dCurve2dInSurfaceOneD *create(void) const;
  virtual ~vec3dCurve2dInSurfaceOneD();
  virtual aFY Y(aFX const &xx) const;
  vectorHandling<renderInterface *> getRender(void) const;
  // dtCurve const * ptrDtCurve( void ) const;
  virtual bool closed(dtInt const &dir) const;

private:
  ptrHandling<dtCurve2d> _dtC2d;
  ptrHandling<dtSurface> _dtS;
};
} // namespace dtOO
#endif /* VEC3DCURVE2DINSURFACEONED_H */
