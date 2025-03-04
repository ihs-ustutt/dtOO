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

#ifndef vec3dTwoD_normalOffset_H
#define vec3dTwoD_normalOffset_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>

namespace dtOO {
class vec3dTwoD;
class vec3dOneD;

class vec3dTwoD_normalOffset {
public:
  dt__classOnlyName(vec3dTwoD_normalOffset);
  vec3dTwoD_normalOffset(
    vec3dTwoD const *const v3d,
    vec3dTwoD const *const thick,
    dtVector3 const &nf,
    dtInt const &nU,
    dtInt const &nV,
    dtInt const &order,
    dtInt const &skinOrderMin,
    dtInt const &skinOrderMax,
    dtInt const &skinNIterations,
    bool const &closeU = false,
    dtReal const &closeSmooth = -1.
  );
  virtual ~vec3dTwoD_normalOffset();
  vec3dTwoD *result(void) const;
  dt__pVH(vec3dOneD) resultWire(void) const;

private:
  dt__pH(vec3dTwoD) _vS;
  dt__pVH(vec3dOneD) _vC;
};
} // namespace dtOO
#endif /* vec3dTwoD_normalOffset_H */
