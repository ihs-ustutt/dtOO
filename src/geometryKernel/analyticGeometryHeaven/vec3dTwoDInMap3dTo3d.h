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

#ifndef vec3dTwoDInMap3dTo3d_H
#define vec3dTwoDInMap3dTo3d_H

#include <dtOOTypeDef.h>

#include "map2dTo3d.h"
#include <dtLinearAlgebra.h>
#include <interfaceHeaven/ptrHandling.h>
#include <logMe/dtMacros.h>

namespace dtOO {
class vec3dTwoD;
class vec3dTwoD;
class map3dTo3d;
class renderInterface;
class dtTransformer;

class vec3dTwoDInMap3dTo3d : public map2dTo3d {
public:
  dt__class(vec3dTwoDInMap3dTo3d, analyticGeometry);
  vec3dTwoDInMap3dTo3d();
  vec3dTwoDInMap3dTo3d(const vec3dTwoDInMap3dTo3d &orig);
  vec3dTwoDInMap3dTo3d(
    vec3dTwoD const *const v2d,
    map3dTo3d const *const m3d,
    bool percentF = false
  );
  virtual ~vec3dTwoDInMap3dTo3d();
  virtual vec3dTwoDInMap3dTo3d *clone(void) const;
  virtual vec3dTwoDInMap3dTo3d *create(void) const;
  virtual vec3dTwoDInMap3dTo3d *cloneTransformed(dtTransformer const *const dtT
  ) const;
  virtual dtPoint3 getPoint(dtReal const &uu, dtReal const &vv) const;
  virtual bool isClosed(dtInt const &dir) const;
  virtual dtReal getMin(dtInt const &dir) const;
  virtual dtReal getMax(dtInt const &dir) const;
  vec3dTwoD const *refToVec3dTwoD(void) const;
  map3dTo3d const *refToMap3dTo3d(void) const;
  virtual vectorHandling<renderInterface *> getExtRender(void) const;
  virtual std::string dumpToString(void) const;

private:
  ptrHandling<vec3dTwoD> _v2d;
  ptrHandling<map3dTo3d> _m3d;
  bool _percentF;
};
} // namespace dtOO
#endif /* vec3dTwoDInMap3dTo3d_H */
