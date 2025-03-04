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

#ifndef vec3dThreeDInMap3dTo3d_H
#define vec3dThreeDInMap3dTo3d_H

#include <dtOOTypeDef.h>

#include "analyticGeometryCompound.h"
#include "map3dTo3d.h"
#include <dtLinearAlgebra.h>
#include <interfaceHeaven/ptrHandling.h>
#include <logMe/dtMacros.h>

namespace dtOO {
class vec3dThreeD;

class vec3dThreeDInMap3dTo3d : public map3dTo3d {
public:
  dt__class(vec3dThreeDInMap3dTo3d, analyticGeometry);
  vec3dThreeDInMap3dTo3d();
  vec3dThreeDInMap3dTo3d(const vec3dThreeDInMap3dTo3d &orig);
  vec3dThreeDInMap3dTo3d(
    vec3dThreeD const *const v3d,
    map3dTo3d const *const m3d,
    bool percentF = false
  );
  virtual ~vec3dThreeDInMap3dTo3d();
  virtual vec3dThreeDInMap3dTo3d *create(void) const;
  virtual vec3dThreeDInMap3dTo3d *clone(void) const;
  virtual vec3dThreeDInMap3dTo3d *
  cloneTransformed(dtTransformer const *const dtT) const;
  virtual dtPoint3
  getPoint(dtReal const &uu, dtReal const &vv, dtReal const &ww) const;
  virtual bool isClosed(dtInt const &dir) const;
  virtual dtReal getMin(dtInt const &dir) const;
  virtual dtReal getMax(dtInt const &dir) const;

private:
  ptrHandling<vec3dThreeD> _v3d;
  ptrHandling<map3dTo3d> _m3d;
  bool _percentF;
};
typedef analyticGeometryCompound<vec3dThreeDInMap3dTo3d>
  vec3dThreeDInMap3dTo3dCompound;
} // namespace dtOO
#endif /* vec3dThreeDInMap3dTo3d_H */
