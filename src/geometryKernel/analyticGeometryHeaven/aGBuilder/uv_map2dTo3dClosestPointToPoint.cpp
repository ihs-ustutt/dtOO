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

#include "uv_map2dTo3dClosestPointToPoint.h"
#include "dtLinearAlgebra.h"

#include <analyticGeometryHeaven/map2dTo3d.h>
#include <attributionHeaven/pointGeometryDist.h>
#include <boost/assign.hpp>
#include <boost/assign/list_of.hpp>
#include <gslMinFloatAttr.h>
#include <interfaceHeaven/staticPropertiesHandler.h>
#include <vector>

namespace dtOO {
uv_map2dTo3dClosestPointToPoint::uv_map2dTo3dClosestPointToPoint(
  map2dTo3d const *const m2d, dtPoint3 const &pXYZ
)
{
  gslMinFloatAttr md(
    dt__pH(pointGeometryDist)(new pointGeometryDist(pXYZ, m2d)),
    ::std::vector<dtPoint2>(
      ::boost::assign::
        list_of(dtPoint2(0.5, 0.5))(dtPoint2(0.0, 0.5))(dtPoint2(1.0, 0.5))(dtPoint2(0.5, 0.0))(dtPoint2(0.0, 0.0))(dtPoint2(1.0, 0.0))(dtPoint2(0.5, 1.0))(dtPoint2(0.0, 1.0))(
          dtPoint2(1.0, 1.0)
        )
    ),
    dtPoint2(0.001, 0.001),
    staticPropertiesHandler::getInstance()->getOptionFloat("xyz_resolution")
  );
  md.perform();
  _closestUV = m2d->uv_percent(dtPoint2(md.result()[0], md.result()[1]));
  _distance = dtLinearAlgebra::distance(m2d->getPoint(_closestUV), pXYZ);
}

uv_map2dTo3dClosestPointToPoint::~uv_map2dTo3dClosestPointToPoint() {}

dtPoint2 uv_map2dTo3dClosestPointToPoint::result(void) { return _closestUV; }

dtReal uv_map2dTo3dClosestPointToPoint::distance(void) { return _distance; }
} // namespace dtOO
