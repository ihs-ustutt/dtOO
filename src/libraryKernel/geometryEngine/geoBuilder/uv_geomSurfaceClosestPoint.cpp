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
    dtPoint2(0.5, 0.5),
    dtPoint2(0.001, 0.001),
    staticPropertiesHandler::getInstance()->getOptionFloat("xyz_resolution")
  );
  md.perform();
  _closestUV = dtS->uv_uvPercent(dtPoint2(md.result()[0], md.result()[1]));
}

uv_geomSurfaceClosestPoint::~uv_geomSurfaceClosestPoint() {}

dtPoint2 uv_geomSurfaceClosestPoint::result(void) { return _closestUV; }
} // namespace dtOO
