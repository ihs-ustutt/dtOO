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

#ifndef vec3dTwoD_closeArithmetic_H
#define vec3dTwoD_closeArithmetic_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>

namespace dtOO {
class vec3dTwoD;

//! Closes gaps between surface mappings with arithmetic surface correction.
class vec3dTwoD_closeArithmetic {
public:
  dt__classOnlyName(vec3dTwoD_closeArithmetic);
  /*!
   * @param v3d Surface mappings whose underlying geometry is corrected.
   * @param dir Surface parameter direction containing the gaps.
   * @param nSegments Number of arithmetic correction segments.
   */
  vec3dTwoD_closeArithmetic(
    dt__pVH(vec3dTwoD) const &v3d, dtInt const &dir, dtInt const &nSegments
  );
  virtual ~vec3dTwoD_closeArithmetic();
  //! Return the corrected surface mappings.
  dt__pVH(vec3dTwoD) result(void);

private:
  dt__pVH(vec3dTwoD) _v3d;
};
} // namespace dtOO
#endif /* vec3dTwoD_closeArithmetic_H */
