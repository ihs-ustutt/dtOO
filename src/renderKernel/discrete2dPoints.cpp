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

#include "discrete2dPoints.h"

#include <interfaceHeaven/ptrHandling.h>
#include <logMe/logMe.h>

namespace dtOO {
discrete2dPoints::discrete2dPoints() : renderInterface() {}

discrete2dPoints::~discrete2dPoints() {}

discrete2dPoints::discrete2dPoints(std::vector<dtPoint2> const &pp)
  : renderInterface()
{
  _p2 = pp;
  std::pair<dtPoint2, dtPoint2> minMax = dtLinearAlgebra::boundingBox(_p2);
  geoBoundMin(0, minMax.first.x());
  geoBoundMin(1, minMax.first.y());
  geoBoundMax(0, minMax.second.x());
  geoBoundMax(1, minMax.second.y());
  //							<< "S0 LINESTYLE 3\n"
  //							<< "SETS LINEWIDTH 2\n"
  //							<< "S0 LINEWIDTH 1\n"
  //							<< "S0 SYMBOL 2\n";
  //		refOptionHandling().setOption("[covise_0]LINESTYLE", "3");
  //		refOptionHandling().setOption("[covise_0]SYMBOL", "2");
}

std::vector<dtPoint2> const &discrete2dPoints::refP2(void) const { return _p2; }
} // namespace dtOO
