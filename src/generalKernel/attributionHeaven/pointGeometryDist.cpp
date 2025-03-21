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

#include "pointGeometryDist.h"
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <logMe/logMe.h>
#include <progHelper.h>
#include <vector>

namespace dtOO {
pointGeometryDist::pointGeometryDist(
  dtPoint3 const &p3, analyticGeometry const *const aG
)
  : _p3(p3), _dimension(aG->dim()), _aG(aG)
{
}

pointGeometryDist::pointGeometryDist(pointGeometryDist const &orig)
  : _p3(orig._p3), _dimension(orig._dimension), _aG(orig._aG)
{
}

pointGeometryDist::~pointGeometryDist() {}

pointGeometryDist *pointGeometryDist::clone(void) const
{
  return new pointGeometryDist(*this);
}

bool pointGeometryDist::outOfRange(::std::vector<dtReal> const &xx) const
{
  if (!floatAtt::outOfRange(xx))
    return false;

  dt__forAllIndex(xx, i)
  {
    // find dimension that is out-of-range
    if ((xx[i] < 0.0) || (xx[i] > 1.0))
    {
      // out-of-range dimension is not closed, so cannot handle
      if (!_aG->isClosed(i))
        return true;
    }
  }

  return false;
}
bool pointGeometryDist::hasGrad(void) const { return true; }

dtReal pointGeometryDist::operator()(std::vector<dtReal> const &xx) const
{
  return dtLinearAlgebra::distance(_aG->getPointPercent(&(xx[0])), _p3);
}

::std::vector<dtReal> pointGeometryDist::grad(::std::vector<dtReal> const &xx
) const
{
  dtPoint3 const ff = _aG->getPointPercent(&(xx[0]));
  dtReal const ffVal = this->operator()(xx);

  ::std::vector<dtVector3> const der = _aG->firstDerPercent(&(xx[0]));

  ::std::vector<dtReal> grad(this->dimension(), 0.0);

  dt__forFromToIndex(0, this->dimension(), i) // parameter coordinate system
  {
    dt__forFromToIndex(0, 3, j) // physical coordinate system
    {
      grad[i] = grad[i] + der[i][j] * (ff[j] - _p3[j]);
    }
    grad[i] = (1. / ffVal) * grad[i];
  }
  return grad;
}

dtInt const &pointGeometryDist::dimension() const { return _dimension; }

dt__C_addCloneForpVH(pointGeometryDist);
} // namespace dtOO
