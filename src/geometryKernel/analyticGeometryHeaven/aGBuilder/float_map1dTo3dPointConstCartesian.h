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

#ifndef float_map1dTo3dPointConstCartesian_H
#define float_map1dTo3dPointConstCartesian_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>

namespace dtOO {
class map1dTo3d;

class float_map1dTo3dPointConstCartesian {
public:
  dt__classOnlyName(float_map1dTo3dPointConstCartesian);
  float_map1dTo3dPointConstCartesian(
    map1dTo3d const *const m1d,
    dtInt const &dir,
    dtReal const &value,
    dtReal const &initGuess
  );
  float_map1dTo3dPointConstCartesian(
    map1dTo3d const *const m1d, dtInt const &dir, dtReal const &value
  );
  virtual ~float_map1dTo3dPointConstCartesian();
  dtReal result(void);

private:
  dtReal _u;
};
} // namespace dtOO
#endif /* float_map1dTo3dPointConstCartesian_H */
