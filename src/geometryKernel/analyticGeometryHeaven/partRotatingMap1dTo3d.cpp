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

#include "partRotatingMap1dTo3d.h"
#include "analyticRotatingMap1dTo3d.h"
#include "map2dTo3dTransformed.h"
#include <geometryEngine/dtCurve.h>
#include <geometryEngine/dtSurface.h>
#include <geometryEngine/geoBuilder/geomCurve_curveRotateConstructOCC.h>

namespace dtOO {
partRotatingMap1dTo3d::partRotatingMap1dTo3d() : rotatingMap1dTo3d() {}

partRotatingMap1dTo3d::partRotatingMap1dTo3d(const partRotatingMap1dTo3d &orig)
  : rotatingMap1dTo3d(orig)
{
  _minB = orig._minB;
  _maxB = orig._maxB;
}

partRotatingMap1dTo3d::~partRotatingMap1dTo3d() {}

partRotatingMap1dTo3d::partRotatingMap1dTo3d(
  dtVector3 const &vv,
  map1dTo3d const *const m1d,
  dtReal const &minB,
  dtReal const &maxB
)
  : rotatingMap1dTo3d(vv, m1d)
{
  _minB = minB;
  _maxB = maxB;
}

partRotatingMap1dTo3d *partRotatingMap1dTo3d::create(void) const
{
  return new partRotatingMap1dTo3d();
}

partRotatingMap1dTo3d *partRotatingMap1dTo3d::clone(void) const
{
  return new partRotatingMap1dTo3d(*this);
}

partRotatingMap1dTo3d *
partRotatingMap1dTo3d::cloneTransformed(dtTransformer const *const dtT) const
{
  return new map2dTo3dTransformed<partRotatingMap1dTo3d>(*this, dtT);
}

bool partRotatingMap1dTo3d::isClosed(dtInt const &dir) const
{
  switch (dir)
  {
  case 0:
    return false;
  default:
    return rotatingMap1dTo3d::isClosed(dir);
  }
}

dtReal partRotatingMap1dTo3d::getMin(dtInt const &dir) const
{
  switch (dir)
  {
  case 0:
    return _minB;
  default:
    return rotatingMap1dTo3d::getMin(dir);
  }
}

dtReal partRotatingMap1dTo3d::getMax(dtInt const &dir) const
{
  switch (dir)
  {
  case 0:
    return _maxB;
  default:
    return rotatingMap1dTo3d::getMax(dir);
  }
}

dtPoint3
partRotatingMap1dTo3d::getPoint(dtReal const &uu, dtReal const &vv) const
{
  return rotatingMap1dTo3d::getPoint(uu, vv);
}

map1dTo3d *partRotatingMap1dTo3d::segmentConstU(dtReal const &uu) const
{
  if (analyticSurface::Is(rotatingMap1dTo3d::constPtrMap1dTo3d()))
  {
    analyticCurve const *s3d =
      analyticCurve::ConstSecureCast(this->constPtrMap1dTo3d());
    geomCurve_curveRotateConstructOCC cCtor(
      s3d->ptrConstDtCurve(), origin(), rotationAxis(), 2.0 * M_PI * _minB
    );
    // dt__pH(dtCurve) cur(cCtor.result());
    return new analyticCurve((dt__pH(dtCurve)(cCtor.result())).get());
  }
  else
    return map2dTo3d::segmentConstU(uu);
}
} // namespace dtOO
