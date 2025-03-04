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

#ifndef map1dTo3d_closeGapsArithmetic_H
#define map1dTo3d_closeGapsArithmetic_H

#include <dtOOTypeDef.h>

#include <logMe/dtMacros.h>

namespace dtOO {
class map1dTo3d;

class map1dTo3d_closeGapsArithmetic {
public:
  dt__classOnlyName(map1dTo3d_closeGapsArithmetic);
  map1dTo3d_closeGapsArithmetic(dt__pVH(map1dTo3d) const &m1d);
  virtual ~map1dTo3d_closeGapsArithmetic();
  dt__pVH(map1dTo3d) result(void);

private:
  dt__pVH(map1dTo3d) _m1d;
};
} // namespace dtOO
#endif /* map1dTo3d_closeGapsArithmetic_H */
