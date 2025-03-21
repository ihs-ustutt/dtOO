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

#include "vec3dThreeDInMap3dTo3d.h"

#include "map3dTo3d.h"
#include "map3dTo3dTransformed.h"
#include <analyticFunctionHeaven/vec3dThreeD.h>
#include <dtTransformerHeaven/dtTransformer.h>
#include <interfaceHeaven/staticPropertiesHandler.h>
#include <logMe/dtMacros.h>
#include <logMe/logMe.h>

namespace dtOO {
vec3dThreeDInMap3dTo3d::vec3dThreeDInMap3dTo3d() : map3dTo3d() {}

vec3dThreeDInMap3dTo3d::vec3dThreeDInMap3dTo3d(
  const vec3dThreeDInMap3dTo3d &orig
)
  : map3dTo3d(orig)
{
  _v3d.reset(orig._v3d->clone());
  _m3d.reset(orig._m3d->clone());
  _percentF = orig._percentF;
}

vec3dThreeDInMap3dTo3d::vec3dThreeDInMap3dTo3d(
  vec3dThreeD const *const v3d, map3dTo3d const *const m3d, bool percentF
)
  : map3dTo3d()
{
  _v3d.reset(v3d->clone());
  _m3d.reset(m3d->clone());
  _percentF = percentF;
}

vec3dThreeDInMap3dTo3d::~vec3dThreeDInMap3dTo3d() {}

dtPoint3 vec3dThreeDInMap3dTo3d::getPoint(
  dtReal const &uu, dtReal const &vv, dtReal const &ww
) const
{
  aFX xx(3, 0);
  xx[0] = uu;
  xx[1] = vv;
  xx[2] = ww;
  dtPoint3 pUVW = _v3d->YdtPoint3(xx);
  if (!_percentF)
  {
    return _m3d->getPoint(pUVW.x(), pUVW.y(), pUVW.z());
  }
  else
  {
    return _m3d->getPointPercent(pUVW.x(), pUVW.y(), pUVW.z());
  }
}

bool vec3dThreeDInMap3dTo3d::isClosed(dtInt const &dir) const { return false; }

dtReal vec3dThreeDInMap3dTo3d::getMin(dtInt const &dir) const
{
  return _v3d->xMin(dir);
}

dtReal vec3dThreeDInMap3dTo3d::getMax(dtInt const &dir) const
{
  return _v3d->xMax(dir);
}

vec3dThreeDInMap3dTo3d *vec3dThreeDInMap3dTo3d::create(void) const
{
  return new vec3dThreeDInMap3dTo3d();
}

vec3dThreeDInMap3dTo3d *vec3dThreeDInMap3dTo3d::clone(void) const
{
  return new vec3dThreeDInMap3dTo3d(*this);
}

vec3dThreeDInMap3dTo3d *
vec3dThreeDInMap3dTo3d::cloneTransformed(dtTransformer const *const dtT) const
{
  return new map3dTo3dTransformed<vec3dThreeDInMap3dTo3d>(*this, dtT);
}
} // namespace dtOO
