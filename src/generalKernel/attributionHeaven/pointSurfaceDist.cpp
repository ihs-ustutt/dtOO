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

#include "pointSurfaceDist.h"

#include <geometryEngine/dtSurface.h>
#include <progHelper.h>
#include <vector>

namespace dtOO {
pointSurfaceDist::pointSurfaceDist(
  dtPoint3 const &p3, dtSurface const *const dtS
)
  : _p3(p3), _dimension(2), _dtS(dtS)
{
}

pointSurfaceDist::pointSurfaceDist(pointSurfaceDist const &orig)
  : _p3(orig._p3), _dimension(orig._dimension), _dtS(orig._dtS)
{
}

pointSurfaceDist::~pointSurfaceDist() {}

pointSurfaceDist *pointSurfaceDist::clone(void) const
{
  return new pointSurfaceDist(*this);
}

dtReal pointSurfaceDist::operator()(std::vector<dtReal> const &xx) const
{
  return dtLinearAlgebra::distance(_dtS->pointPercent(xx[0], xx[1]), _p3);
}

dtInt const &pointSurfaceDist::dimension() const { return _dimension; }

dt__C_addCloneForpVH(pointSurfaceDist);
} // namespace dtOO
