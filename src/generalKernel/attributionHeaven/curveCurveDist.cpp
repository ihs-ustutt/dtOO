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

#include "curveCurveDist.h"

#include <geometryEngine/dtCurve.h>
#include <progHelper.h>
#include <vector>

namespace dtOO {
curveCurveDist::curveCurveDist(dtCurve const *const c0, dtCurve const *const c1)
  : _c0(c0), _c1(c1), _dimension(2)
{
}

curveCurveDist::curveCurveDist(curveCurveDist const &orig)
  : _c0(orig._c0), _c1(orig._c1), _dimension(orig._dimension)
{
}

curveCurveDist::~curveCurveDist() {}

curveCurveDist *curveCurveDist::clone(void) const
{
  return new curveCurveDist(*this);
}

dtReal curveCurveDist::operator()(std::vector<dtReal> const &xx) const
{
  return dtLinearAlgebra::distance(
    _c0->pointPercent(xx[0]), _c1->pointPercent(xx[1])
  );
}

dtInt const &curveCurveDist::dimension() const { return _dimension; }

dt__C_addCloneForpVH(curveCurveDist);
} // namespace dtOO
