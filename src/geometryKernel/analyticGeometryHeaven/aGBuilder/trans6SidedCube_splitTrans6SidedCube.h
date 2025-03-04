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

#ifndef trans6SidedCube_splitTrans6SidedCube_H
#define trans6SidedCube_splitTrans6SidedCube_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <interfaceHeaven/vectorHandling.h>
#include <logMe/dtMacros.h>

namespace dtOO {
class trans6SidedCube;
class analyticGeometry;

class trans6SidedCube_splitTrans6SidedCube {
public:
  dt__classOnlyName(trans6SidedCube_splitTrans6SidedCube);
  trans6SidedCube_splitTrans6SidedCube(
    trans6SidedCube const *const t6, dtPoint3 const &splitPoint
  );
  virtual ~trans6SidedCube_splitTrans6SidedCube();
  vectorHandling<trans6SidedCube *> result(void);
  vectorHandling<analyticGeometry *> resultGeometries(void);

private:
  dt__pVH(trans6SidedCube) _t6;
  dt__pVH(analyticGeometry) _aG;
};
} // namespace dtOO
#endif /* trans6SidedCube_splitTrans6SidedCube_H */
