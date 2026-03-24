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

#ifndef map1dTo3d_findLoop_H
#define map1dTo3d_findLoop_H

#include <dtOOTypeDef.h>

#include <interfaceHeaven/vectorHandling.h>
#include <logMe/dtMacros.h>

namespace dtOO {
class analyticGeometry;
class map1dTo3d;

class map1dTo3d_findLoop {
public:
  dt__classOnlyName(map1dTo3d_findLoop);
  map1dTo3d_findLoop(dt__pVH(map1dTo3d) const &m1ds);
  map1dTo3d_findLoop(vectorHandling<analyticGeometry *> const m1ds);
  virtual ~map1dTo3d_findLoop();
  std::vector<dtInt> result(void);

private:
  std::vector<dtInt> _orientedCurves;
};
} // namespace dtOO
#endif /* map1dTo3d_findLoop_H */
