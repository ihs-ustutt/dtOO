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

#include "scaOneDPolyIElSize.h"

#include <analyticFunctionHeaven/scaOneDPolyInterface.h>
#include <progHelper.h>
#include <vector>

namespace dtOO {
scaOneDPolyIElSize::scaOneDPolyIElSize(
  scaOneDPolyInterface *const polyI,
  dtReal const &x0,
  dtReal const &x1,
  dtReal const &lByS0,
  dtReal const &lByS1
)
  : _polyI(polyI), _x0(x0), _x1(x1), _lByS0(lByS0), _lByS1(lByS1),
    _dimension(polyI->nDOF())
{
}

scaOneDPolyIElSize::scaOneDPolyIElSize(scaOneDPolyIElSize const &orig)
  : _polyI(orig._polyI), _x0(orig._x0), _x1(orig._x1), _lByS0(orig._lByS0),
    _lByS1(orig._lByS1), _dimension(orig._dimension)
{
}

scaOneDPolyIElSize::~scaOneDPolyIElSize() {}

scaOneDPolyIElSize *scaOneDPolyIElSize::clone(void) const
{
  return new scaOneDPolyIElSize(*this);
}

dtReal scaOneDPolyIElSize::operator()(std::vector<dtReal> const &xx) const
{
  _polyI->setDOF(xx);

  dtReal ret0 = 0.0;
  dtReal ret1 = 0.0;
  if (_lByS0 > 0.0)
  {
    ret0 = fabs(_lByS0 * _polyI->YFloat(_x0) - 1.0);
  }
  if (_lByS1 > 0.0)
  {
    ret1 = fabs(_lByS1 * (1. - _polyI->YFloat(_x1)) - 1.0);
  }
  return ret0 + ret1;
}

dtInt const &scaOneDPolyIElSize::dimension() const { return _dimension; }

dt__C_addCloneForpVH(scaOneDPolyIElSize);
} // namespace dtOO
