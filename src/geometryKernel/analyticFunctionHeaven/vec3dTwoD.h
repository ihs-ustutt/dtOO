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

#ifndef VEC3DTWOD_H
#define VEC3DTWOD_H

#include <dtOOTypeDef.h>

#include "vec3dFunction.h"
#include <dtLinearAlgebra.h>
#include <interfaceHeaven/vectorHandling.h>
#include <logMe/dtMacros.h>

namespace dtOO {
class renderInterface;
class dtTransformer;

class vec3dTwoD : public vec3dFunction {
public:
  dt__class(vec3dTwoD, analyticFunction);
  vec3dTwoD();
  vec3dTwoD(const vec3dTwoD &orig);
  virtual ~vec3dTwoD();
  virtual vec3dTwoD *clone(void) const = 0;
  virtual vec3dTwoD *cloneTransformed(dtTransformer const *const dtT) const = 0;
  virtual vec3dTwoD *create(void) const = 0;
  virtual aFY Y(aFX const &xx) const = 0;
  virtual aFY Y(dtReal const &x0, dtReal const &x1) const;
  virtual dtInt xDim(void) const;
  void setMax(int const &dir, dtReal const &max);
  void setMin(int const &dir, dtReal const &min);
  virtual dtReal xMin(dtInt const &dir) const;
  virtual dtReal xMax(dtInt const &dir) const;
  virtual std::vector<dtVector3> DYdtVector3(aFX const &xx) const;
  std::vector<dtVector3> DYdtVector3(dtReal const &x0, dtReal const &x1) const;
  dtVector3 unitNdtVector3(aFX const &xx) const;
  dtVector3 unitNdtVector3Percent(aFX const &xx) const;
  using analyticFunction::percent_x;
  using analyticFunction::x_percent;
  aFX x_percent(dtReal const &x0, dtReal const &x1) const;
  aFX percent_x(dtReal const &x0, dtReal const &x1) const;
  dtPoint3 YdtPoint3(aFX const &xx) const;
  dtPoint3 YdtPoint3(dtReal const &x0, dtReal const &x1) const;
  dtPoint3 YdtPoint3Percent(aFX const &xx) const;
  dtPoint3 YdtPoint3Percent(dtReal const &x0, dtReal const &x1) const;
  vectorHandling<renderInterface *> getRender(void) const;
  aFX operator%(const aFX &percent) const;

private:
  dtReal _min[2];
  dtReal _max[2];
};
dt__H_addCloneForpVH(vec3dTwoD);
} // namespace dtOO
#endif /* VEC3DTWOD_H */
