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

#include "pointCurve2dOneDDist.h"

#include <geometryEngine/dtCurve2d.h>
#include <progHelper.h>
#include <vector>

namespace dtOO {
pointCurve2dOneDDist::pointCurve2dOneDDist(
  dtReal const &value, dtCurve2d const *const dtC, dtInt const &dir
)
  : _value(value), _dir(dir), _dimension(1), _dtC(dtC)
{
}

pointCurve2dOneDDist::pointCurve2dOneDDist(pointCurve2dOneDDist const &orig)
  : _value(orig._value), _dir(orig._dir), _dimension(orig._dimension),
    _dtC(orig._dtC)
{
}

pointCurve2dOneDDist::~pointCurve2dOneDDist() {}

pointCurve2dOneDDist *pointCurve2dOneDDist::clone(void) const
{
  return new pointCurve2dOneDDist(*this);
}

dtReal pointCurve2dOneDDist::operator()(std::vector<dtReal> const &xx) const
{
  return fabs(_dtC->pointPercent(xx[0])[_dir] - _value);
}

dtInt const &pointCurve2dOneDDist::dimension() const { return _dimension; }

dt__C_addCloneForpVH(pointCurve2dOneDDist);
} // namespace dtOO
