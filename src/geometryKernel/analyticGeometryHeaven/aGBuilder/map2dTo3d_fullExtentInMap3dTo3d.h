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

#ifndef map2dTo3d_fullExtentInMap3dTo3d_H
#define map2dTo3d_fullExtentInMap3dTo3d_H

#include <dtOOTypeDef.h>

#include <logMe/dtMacros.h>

namespace dtOO {
class map2dTo3d;

class map2dTo3d_fullExtentInMap3dTo3d {
public:
  dt__classOnlyName(map2dTo3d_fullExtentInMap3dTo3d);
  map2dTo3d_fullExtentInMap3dTo3d(
    map2dTo3d const *const m2d, dtInt const &m2dParDir, dtInt const &toM3dParDir
  );
  virtual ~map2dTo3d_fullExtentInMap3dTo3d();
  map2dTo3d *result(void);

private:
  dt__pH(map2dTo3d) _m2d;
};
} // namespace dtOO
#endif /* map2dTo3d_fullExtentInMap3dTo3d_H */
