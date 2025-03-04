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

#include "radialTranslatingMap2dTo3d.h"
#include "map2dTo3d.h"
#include "map3dTo3dTransformed.h"
#include <dtTransformerHeaven/dtTransformer.h>
#include <geometryEngine/dtSurface.h>
#include <logMe/logMe.h>

namespace dtOO {
radialTranslatingMap2dTo3d::radialTranslatingMap2dTo3d() : map3dTo3d() {}

radialTranslatingMap2dTo3d::radialTranslatingMap2dTo3d(
  const radialTranslatingMap2dTo3d &orig
)
  : map3dTo3d(orig)
{
  _m2d.reset(orig._m2d->clone());
  _vv = orig._vv;
  _distance = orig._distance;
}

radialTranslatingMap2dTo3d::~radialTranslatingMap2dTo3d() {}

radialTranslatingMap2dTo3d::radialTranslatingMap2dTo3d(
  dtVector3 const &vv, dtReal const &distance, map2dTo3d const *const m2d
)
  : map3dTo3d()
{
  if (m2d->isClosedU() || m2d->isClosedV())
  {
    dt__throw(
      radialTranslatingMap2dTo3d(),
      << "Surface is closed. This is not yet supported." << std::endl
      << dt__eval(m2d->isClosedU()) << std::endl
      << dt__eval(m2d->isClosedV())
    );
  }
  _distance = distance;
  _vv = dtLinearAlgebra::normalize(vv);
  _m2d.reset(m2d->clone());
}

radialTranslatingMap2dTo3d *radialTranslatingMap2dTo3d::create(void) const
{
  return new radialTranslatingMap2dTo3d();
}

radialTranslatingMap2dTo3d *radialTranslatingMap2dTo3d::clone(void) const
{
  return new radialTranslatingMap2dTo3d(*this);
}

radialTranslatingMap2dTo3d *
radialTranslatingMap2dTo3d::cloneTransformed(dtTransformer const *const dtT
) const
{
  return new map3dTo3dTransformed<radialTranslatingMap2dTo3d>(*this, dtT);
}

bool radialTranslatingMap2dTo3d::isClosed(dtInt const &dir) const
{
  switch (dir)
  {
  case 0:
    return false;
  case 1:
    return false;
  case 2:
    return false;
  default:
    dt__throw(
      isClosed(),
      << dt__eval(dir) << std::endl
      << "dir should be 0, 1 or 2."
    );
  }
}

dtReal radialTranslatingMap2dTo3d::getMin(dtInt const &dir) const
{
  switch (dir)
  {
  case 0:
    return _m2d->getUMin();
  case 1:
    return _m2d->getVMin();
  case 2:
    return 0.;
  default:
    dt__throw(
      getMin(),
      << dt__eval(dir) << std::endl
      << "dir should be 0, 1 or 2."
    );
  }
}

dtReal radialTranslatingMap2dTo3d::getMax(dtInt const &dir) const
{
  switch (dir)
  {
  case 0:
    return _m2d->getUMax();
  case 1:
    return _m2d->getVMax();
  case 2:
    return 1.;
  default:
    dt__throw(
      getMax(),
      << dt__eval(dir) << std::endl
      << "dir should be 0, 1 or 2."
    );
  }
}

dtPoint3 radialTranslatingMap2dTo3d::getPoint(
  dtReal const &uu, dtReal const &vv, dtReal const &ww
) const
{
  dtPoint3 P_0 = _m2d->getPoint(uu, vv);
  dtVector3 RR(P_0.x(), P_0.y(), P_0.z());
  dtVector3 RR_z = dtLinearAlgebra::dotProduct(RR, _vv) * _vv;
  return P_0 + ww * dtLinearAlgebra::normalize(RR - RR_z) * _distance;
}

std::string radialTranslatingMap2dTo3d::dumpToString(void) const
{
  std::stringstream ss;

  ss << dt__dumpToString(
    << "_m2d = " << _m2d->virtualClassName() << std::endl
    << _m2d->dumpToString() << std::endl
    << dt__eval(_distance) << std::endl
    << dt__vector3d(_vv)
  );

  return ss.str();
}
} // namespace dtOO
