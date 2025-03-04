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

#ifndef uv_map2dTo3dClosestPointToPoint_H
#define uv_map2dTo3dClosestPointToPoint_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>

namespace dtOO {
class map2dTo3d;

class uv_map2dTo3dClosestPointToPoint {
public:
  dt__classOnlyName(uv_map2dTo3dClosestPointToPoint);
  uv_map2dTo3dClosestPointToPoint(
    map2dTo3d const *const m2d, dtPoint3 const &pXYZ
  );
  virtual ~uv_map2dTo3dClosestPointToPoint();
  dtPoint2 result(void);
  dtReal distance(void);

private:
  dtPoint2 _closestUV;
  dtReal _distance;
};
} // namespace dtOO
#endif /* uv_map2dTo3dClosestPointToPoint_H */
