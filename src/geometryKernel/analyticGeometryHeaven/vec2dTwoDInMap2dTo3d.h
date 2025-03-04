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

#ifndef VEC2DTWODINMAP2DTO3D_H
#define VEC2DTWODINMAP2DTO3D_H

#include <dtOOTypeDef.h>

#include "map2dTo3d.h"
#include <dtLinearAlgebra.h>
#include <interfaceHeaven/ptrHandling.h>
#include <logMe/dtMacros.h>

namespace dtOO {
class vec2dTwoD;
class dtTransformer;

class vec2dTwoDInMap2dTo3d : public map2dTo3d {
public:
  dt__class(vec2dTwoDInMap2dTo3d, analyticGeometry);
  vec2dTwoDInMap2dTo3d();
  vec2dTwoDInMap2dTo3d(const vec2dTwoDInMap2dTo3d &orig);
  vec2dTwoDInMap2dTo3d(
    vec2dTwoD const *const v2d2d, map2dTo3d const *const m2d
  );
  vec2dTwoDInMap2dTo3d(
    vec2dTwoD const *const v2d2d,
    map2dTo3d const *const m2d,
    bool const &percentF
  );
  virtual ~vec2dTwoDInMap2dTo3d();
  //
  // overload
  //
  virtual vec2dTwoDInMap2dTo3d *clone(void) const;
  virtual vec2dTwoDInMap2dTo3d *create(void) const;
  virtual vec2dTwoDInMap2dTo3d *cloneTransformed(dtTransformer const *const dtT
  ) const;
  virtual dtPoint3 getPoint(dtReal const &uu, dtReal const &vv) const;
  virtual bool isClosed(dtInt const &dir) const;
  virtual dtReal getMin(dtInt const &dir) const;
  virtual dtReal getMax(dtInt const &dir) const;
  //
  //
  //
  vec2dTwoD const *ptrToVec2dTwoD(void) const;
  map2dTo3d const *ptrToMap2dTo3d(void) const;

private:
  dt__pH(vec2dTwoD) _v2d;
  dt__pH(map2dTo3d) _m2d;
  bool _percentF;
};
} // namespace dtOO
#endif /* VEC2DTWODINMAP2DTO3D_H */
