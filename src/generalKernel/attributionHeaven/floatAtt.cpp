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

#include "floatAtt.h"
#include <limits>
#include <logMe/dtMacros.h>
#include <logMe/logMe.h>
#include <progHelper.h>

namespace dtOO {
floatAtt::floatAtt() {}

floatAtt::floatAtt(floatAtt const &orig) {}

floatAtt::~floatAtt() {}

bool floatAtt::outOfRange(::std::vector<dtReal> const &xx) const
{
  dt__forAllIndex(xx, i)
  {
    if ((xx[i] < 0.0) || (xx[i] > 1.0) || isnan(xx[i]))
    {
      dt__warnIf(isnan(xx[i]), outOfRange()) return true;
    }
  }
  return false;
}

dtReal floatAtt::outOfRangeResult() const
{
  return ::std::numeric_limits<dtReal>::max();
}

dtReal floatAtt::rangeCheckAndCall(::std::vector<dtReal> const &xx) const
{
  if (outOfRange(xx))
  {
    return outOfRangeResult();
  }
  return this->operator()(xx);
}
dt__C_addCloneForpVH(floatAtt);
} // namespace dtOO
