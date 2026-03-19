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

#include "trans4SidedFace.h"

#include "aGBuilder/map1dTo3d_orderLoop.h"
#include "dtLinearAlgebra.h"
#include <logMe/dtMacros.h>
#include "map1dTo3d.h"
#include "map2dTo3dTransformed.h"
#include <dtTransformerHeaven/dtTransformer.h>
#include <logMe/logMe.h>

namespace dtOO {
trans4SidedFace::trans4SidedFace() : map2dTo3d() {}

trans4SidedFace::trans4SidedFace(const trans4SidedFace &orig) : map2dTo3d(orig)
{
  dt__forAllRefAuto(orig._curves, curve) { _curves.push_back(curve.clone()); }
  _points = orig._points;
}

trans4SidedFace::trans4SidedFace(
  map1dTo3d const *const c0,
  map1dTo3d const *const c1,
  map1dTo3d const *const c2,
  map1dTo3d const *const c3
)
  : map2dTo3d()
{
  _curves.push_back(c0->clone());
  _curves.push_back(c1->clone());
  _curves.push_back(c2->clone());
  _curves.push_back(c3->clone());
  _curves = map1dTo3d_orderLoop(_curves).result();
  _points.push_back(_curves[0].getPointPercent(0.0));
  _points.push_back(_curves[0].getPointPercent(1.0));
  _points.push_back(_curves[2].getPointPercent(0.0));
  _points.push_back(_curves[2].getPointPercent(1.0));
}

trans4SidedFace *trans4SidedFace::create(void) const
{
  return new trans4SidedFace();
}

trans4SidedFace *trans4SidedFace::clone(void) const
{
  return new trans4SidedFace(*this);
}

trans4SidedFace *
trans4SidedFace::cloneTransformed(dtTransformer const *const dtT) const
{
  return new map2dTo3dTransformed<trans4SidedFace>(*this, dtT);
}

trans4SidedFace::~trans4SidedFace() {}

dtPoint3 trans4SidedFace::getPoint(dtReal const &uu, dtReal const &vv) const
{
  //
  // naming and implementation accodring to
  //   https://en.wikipedia.org/wiki/Transfinite_interpolation
  //
  dtVector3 const C1 =
    dtLinearAlgebra::toDtVector3(_curves[0].getPointPercent(uu));
  dtVector3 const C2 =
    dtLinearAlgebra::toDtVector3(_curves[1].getPointPercent(vv));
  // C3 and C4 are reversed; therefore get points at parameter coordinates
  // (1.-uu) and (1-vv.)
  dtVector3 const C3 =
    dtLinearAlgebra::toDtVector3(_curves[2].getPointPercent(1. - uu));
  dtVector3 const C4 =
    dtLinearAlgebra::toDtVector3(_curves[3].getPointPercent(1. - vv));
  dtVector3 const &P12 = dtLinearAlgebra::toDtVector3(_points[0]);
  dtVector3 const &P14 = dtLinearAlgebra::toDtVector3(_points[1]);
  dtVector3 const &P34 = dtLinearAlgebra::toDtVector3(_points[2]);
  dtVector3 const &P32 = dtLinearAlgebra::toDtVector3(_points[3]);

  return dtLinearAlgebra::toDtPoint3(
    (1. - vv) * C1 + vv * C3 + (1. - uu) * C2 + uu * C4 -
    ((1. - uu) * (1. - vv) * P12 + uu * vv * P34 + uu * (1. - vv) * P14 +
     (1. - uu) * vv * P32)
  );
}

bool trans4SidedFace::isClosed(dtInt const &dir) const
{
  switch (dir)
  {
  case 0:
    return false;
  case 1:
    return false;
  default:
    dt__throwUnexpected(isClosed());
  }
}

dtReal trans4SidedFace::getMin(dtInt const &dir) const
{
  switch (dir)
  {
  case 0:
    return 0.;
  case 1:
    return 0.;
  default:
    dt__throwUnexpected(getMin());
  }
}

dtReal trans4SidedFace::getMax(dtInt const &dir) const
{
  switch (dir)
  {
  case 0:
    return 1.;
  case 1:
    return 1.;
  default:
    dt__throwUnexpected(getMax());
  }
}

} // namespace dtOO
