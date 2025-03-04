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

#include "solid2dLine.h"

#include <interfaceHeaven/ptrHandling.h>
#include <logMe/logMe.h>

namespace dtOO {
solid2dLine::solid2dLine() : discrete2dPoints() {}

solid2dLine::~solid2dLine() {}

solid2dLine::solid2dLine(std::vector<dtPoint2> const &pp) : discrete2dPoints(pp)
{
}
} // namespace dtOO
