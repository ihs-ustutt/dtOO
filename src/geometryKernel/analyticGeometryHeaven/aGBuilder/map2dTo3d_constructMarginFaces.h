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

#ifndef MAP2DTO3D_CONSTRUCTMARGINFACES_H
#define MAP2DTO3D_CONSTRUCTMARGINFACES_H

#include <dtOOTypeDef.h>

#include <logMe/dtMacros.h>

namespace dtOO {
class map2dTo3d;

/**
 * \code
 * (v)
 *  ^
 *  |
 *
 *  (0,1)            (1,1)             faces:
 *  o--------------------o             <0>: 0/1/2/3
 *  |\     <3>          /|             <1>: (0,0)/(1,0)/1/0
 *  | o----------------o |             <2>: (1,0)/(1,1)/2/1
 *  | |3              2| |             <3>: (1,1)/(0,1)/3/2
 *  <4>      <0>       <2>             <4>: (0,1)/(0,0)/0/3
 *  | |0              1| |
 *  | o----------------o |
 *  |/      <1>         \|
 *  o--------------------o  --> (u)
 *  (0,0)            (1,0)
 * \endcode
 */
class map2dTo3d_constructMarginFaces {
public:
  dt__classOnlyName(map2dTo3d_constructMarginFaces);
  map2dTo3d_constructMarginFaces(
    map2dTo3d const *const m2d, dtReal const &width
  );
  virtual ~map2dTo3d_constructMarginFaces();
  dt__pVH(map2dTo3d) result(void);

private:
  dt__pVH(map2dTo3d) _m2dV;
};
} // namespace dtOO
#endif /* MAP2DTO3D_CONSTRUCTMARGINFACES_H */
