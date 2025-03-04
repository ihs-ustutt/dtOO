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

#include "scaFunction.h"
#include <logMe/dtMacros.h>
#include <logMe/logMe.h>

namespace dtOO {
scaFunction::scaFunction() : analyticFunction() {}

scaFunction::scaFunction(scaFunction const &orig) : analyticFunction(orig) {}

scaFunction::~scaFunction() {}

dtInt scaFunction::yDim(void) const { return 1; }

aFY scaFunction::Y(aFX const &xx) const
{
  aFY yy(1, 0.);
  yy[0] = YFloat(xx);

  return yy;
}
dt__C_addCloneForpVH(scaFunction);
} // namespace dtOO
