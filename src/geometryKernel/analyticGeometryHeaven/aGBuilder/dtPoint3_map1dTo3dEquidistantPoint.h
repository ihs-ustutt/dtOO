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

#ifndef dtPoint3_map1dTo3dEquidistantPoint_H
#define dtPoint3_map1dTo3dEquidistantPoint_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <vector>

namespace dtOO {
class map1dTo3d;

class dtPoint3_map1dTo3dEquidistantPoint {
public:
  dt__classOnlyName(dtPoint3_map1dTo3dEquidistantPoint);
  dtPoint3_map1dTo3dEquidistantPoint(
    map1dTo3d const *const m1d, dtInt const &nPoints
  );
  virtual ~dtPoint3_map1dTo3dEquidistantPoint();
  std::vector<dtPoint3> result(void);

private:
  std::vector<dtPoint3> _ppXYZ;
};
} // namespace dtOO
#endif /* dtPoint3_map1dTo3dEquidistantPoint_H */
