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

#include "bool_map1dTo3dInMap2dTo3d.h"

#include "uv_map2dTo3dClosestPointToPoint.h"
#include <analyticGeometryHeaven/map1dTo3d.h>
#include <analyticGeometryHeaven/map2dTo3d.h>
#include <interfaceHeaven/staticPropertiesHandler.h>
#include <limits>

namespace dtOO {
bool_map1dTo3dInMap2dTo3d::bool_map1dTo3dInMap2dTo3d(
  map1dTo3d const *const m1d, map2dTo3d const *const m2d
)
  : _inside(false)
{
  // check distance to start and end point
  uv_map2dTo3dClosestPointToPoint p0(m2d, m1d->getPointPercent(0.0));
  if (!analyticGeometry::inXYZTolerance(p0.distance()))
    return;
  uv_map2dTo3dClosestPointToPoint p1(m2d, m1d->getPointPercent(1.0));
  if (!analyticGeometry::inXYZTolerance(p1.distance()))
    return;

  _inside = true;

  _points.first = p0.result();
  _points.second = p1.result();
}

bool_map1dTo3dInMap2dTo3d::~bool_map1dTo3dInMap2dTo3d() {}

bool bool_map1dTo3dInMap2dTo3d::result(void) { return _inside; }

std::pair<dtPoint2, dtPoint2> bool_map1dTo3dInMap2dTo3d::points(void) const
{
  dt__throwIf(_inside == false, points());
  return _points;
}
} // namespace dtOO
