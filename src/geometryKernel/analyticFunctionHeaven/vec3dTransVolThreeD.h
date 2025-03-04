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

#ifndef VEC3DTRANSVOLTHREED_H
#define VEC3DTRANSVOLTHREED_H

#include <dtOOTypeDef.h>

#include "analyticFunctionCompound.h"
#include "vec3dThreeD.h"
#include "vec3dTwoD.h"
#include <dtLinearAlgebra.h>
#include <interfaceHeaven/ptrVectorHandling.h>
#include <logMe/dtMacros.h>

namespace dtOO {
class vec3dTwoD;
class dtTransformer;

class vec3dTransVolThreeD : public vec3dThreeD {
public:
  dt__class(vec3dTransVolThreeD, analyticFunction);
  vec3dTransVolThreeD();
  vec3dTransVolThreeD(const vec3dTransVolThreeD &orig);
  vec3dTransVolThreeD(
    vec3dTwoD const *const v3d2d0,
    vec3dTwoD const *const v3d2d1,
    vec3dTwoD const *const v3d2d2,
    vec3dTwoD const *const v3d2d3,
    vec3dTwoD const *const v3d2d4,
    vec3dTwoD const *const v3d2d5
  );
  virtual vec3dTransVolThreeD *clone(void) const;
  virtual vec3dTransVolThreeD *cloneTransformed(dtTransformer const *const dtT
  ) const;
  virtual vec3dTransVolThreeD *create(void) const;
  virtual ~vec3dTransVolThreeD();
  virtual aFY Y(aFX const &xx) const;
  virtual bool closed(dtInt const &dir) const;

private:
  ptrVectorHandling<vec3dTwoD> _v3dTwoD;
  dtVector3 _v3dTwoD_1_0_0;
  dtVector3 _v3dTwoD_1_0_1;
  dtVector3 _v3dTwoD_1_1_0;
  dtVector3 _v3dTwoD_1_1_1;
  dtVector3 _v3dTwoD_3_0_0;
  dtVector3 _v3dTwoD_3_0_1;
  dtVector3 _v3dTwoD_3_1_0;
  dtVector3 _v3dTwoD_3_1_1;
};
typedef analyticFunctionCompound<vec3dTransVolThreeD>
  vec3dTransVolThreeDCompound;
} // namespace dtOO
#endif /* VEC3DTRANSVOLTHREED_H */
