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

#include "scaOneDInMap1dTo3d.h"
#include "map1dTo3dTransformed.h"
#include <analyticFunctionHeaven/scaOneD.h>
#include <dtTransformerHeaven/dtTransformer.h>
#include <logMe/logMe.h>

namespace dtOO {
scaOneDInMap1dTo3d::scaOneDInMap1dTo3d() : map1dTo3d() {}

scaOneDInMap1dTo3d::scaOneDInMap1dTo3d(
  scaOneD const *const s1d, map1dTo3d const *const m1d
)
  : map1dTo3d()
{
  _s1d.reset(s1d->clone());
  _m1d.reset(m1d->clone());
  _percentF = false;
}

scaOneDInMap1dTo3d::scaOneDInMap1dTo3d(
  scaOneD const *const s1d, map1dTo3d const *const m1d, bool const &percentF
)
  : map1dTo3d()
{
  _s1d.reset(s1d->clone());
  _m1d.reset(m1d->clone());
  _percentF = percentF;
}

scaOneDInMap1dTo3d::scaOneDInMap1dTo3d(const scaOneDInMap1dTo3d &orig)
  : map1dTo3d(orig)
{
  _s1d.reset(orig._s1d->clone());
  _m1d.reset(orig._m1d->clone());
  _percentF = orig._percentF;
}

scaOneDInMap1dTo3d::~scaOneDInMap1dTo3d() {}

scaOneDInMap1dTo3d *scaOneDInMap1dTo3d::clone(void) const
{
  return new scaOneDInMap1dTo3d(*this);
}

scaOneDInMap1dTo3d *
scaOneDInMap1dTo3d::cloneTransformed(dtTransformer const *const dtT) const
{
  return new map1dTo3dTransformed<scaOneDInMap1dTo3d>(*this, dtT);
}

scaOneDInMap1dTo3d *scaOneDInMap1dTo3d::create(void) const
{
  return new scaOneDInMap1dTo3d();
}

dtPoint3 scaOneDInMap1dTo3d::getPoint(dtReal const &uu) const
{
  if (_percentF)
  {
    return _m1d->getPoint(_m1d->u_percent(_s1d->YFloat(uu)));
  }
  else
  {
    return _m1d->getPoint(_s1d->YFloat(uu));
  }
}

/**
 * @todo This check is not enough. Maybe a general check on base class?
 */
bool scaOneDInMap1dTo3d::isClosed(dtInt const &dir) const
{
  dt__throwIf(dir != 0, isClosed());

  return false;
}

dtReal scaOneDInMap1dTo3d::getMin(dtInt const &dir) const
{
  return _s1d->xMin(dir);
}

dtReal scaOneDInMap1dTo3d::getMax(dtInt const &dir) const
{
  return _s1d->xMax(dir);
}

scaOneD const *scaOneDInMap1dTo3d::ptrToScaOneD(void) const
{
  return _s1d.get();
}

map1dTo3d const *scaOneDInMap1dTo3d::ptrToMap1dTo3d(void) const
{
  return _m1d.get();
}
} // namespace dtOO
