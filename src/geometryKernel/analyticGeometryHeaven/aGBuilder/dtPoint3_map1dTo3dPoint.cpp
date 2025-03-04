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

#include "dtPoint3_map1dTo3dPoint.h"

#include <analyticGeometryHeaven/map1dTo3d.h>

namespace dtOO {
dtPoint3_map1dTo3dPoint::dtPoint3_map1dTo3dPoint(
  map1dTo3d const *const m1d, dtInt const &nPoints
)
{
  _ppXYZ.resize(nPoints);
  dtReal dist = 1. / (nPoints - 1);
  for (int ii = 0; ii < nPoints; ii++)
  {
    dt__toFloat(dtReal iiF, ii);
    _ppXYZ[ii] = m1d->getPoint(*m1d % (dist * iiF));
  }
}

dtPoint3_map1dTo3dPoint::~dtPoint3_map1dTo3dPoint() {}

std::vector<dtPoint3> dtPoint3_map1dTo3dPoint::result(void) { return _ppXYZ; }
} // namespace dtOO
