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

#ifndef dtPoint3_map2dTo3dPoint_H
#define dtPoint3_map2dTo3dPoint_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <interfaceHeaven/twoDArrayHandling.h>
#include <logMe/dtMacros.h>

namespace dtOO {
class map2dTo3d;

class dtPoint3_map2dTo3dPoint {
public:
  dt__classOnlyName(dtPoint3_map2dTo3dPoint);
  dtPoint3_map2dTo3dPoint(
    map2dTo3d const *const m2d, dtInt const &nU, dtInt const &nV
  );
  virtual ~dtPoint3_map2dTo3dPoint();
  twoDArrayHandling<dtPoint3> result(void);

private:
  twoDArrayHandling<dtPoint3> _pp;
};
} // namespace dtOO
#endif /* dtPoint3_map2dTo3dPoint_H */
