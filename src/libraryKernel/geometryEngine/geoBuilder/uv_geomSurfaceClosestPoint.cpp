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

#include "uv_geomSurfaceClosestPoint.h"

#include <attributionHeaven/pointSurfaceDist.h>
#include <boost/assign.hpp>
#include <boost/assign/list_of.hpp>
#include <geometryEngine/dtSurface.h>
#include <gslMinFloatAttr.h>
#include <interfaceHeaven/staticPropertiesHandler.h>

namespace dtOO {
uv_geomSurfaceClosestPoint::uv_geomSurfaceClosestPoint(
  dtSurface const *const dtS, dtPoint3 const &pXYZ
)
{
  gslMinFloatAttr md(
    dt__pH(pointSurfaceDist)(new pointSurfaceDist(pXYZ, dtS)),
    // clang-format off
    ::std::vector<dtPoint2>(
      ::boost::assign::list_of
        (dtPoint2(0.50, 0.50))
        (dtPoint2(0.75, 0.50))
        (dtPoint2(0.25, 0.50))
        (dtPoint2(0.50, 0.75))
        (dtPoint2(0.75, 0.75))
        (dtPoint2(0.25, 0.75))
        (dtPoint2(0.50, 0.25))
        (dtPoint2(0.75, 0.25))
        (dtPoint2(0.25, 0.25))
    ),
    // clang-format on
    dtPoint2(0.001, 0.001),
    staticPropertiesHandler::getInstance()->getOptionFloat("xyz_resolution")
  );
  md.perform();
  _closestUV = dtS->uv_uvPercent(dtPoint2(md.result()[0], md.result()[1]));
}

uv_geomSurfaceClosestPoint::~uv_geomSurfaceClosestPoint() {}

dtPoint2 uv_geomSurfaceClosestPoint::result(void) { return _closestUV; }
} // namespace dtOO
