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

#ifndef VEC2DTWOD_H
#define VEC2DTWOD_H

#include <dtOOTypeDef.h>

#include "vec2dFunction.h"
#include <dtLinearAlgebra.h>
#include <interfaceHeaven/vectorHandling.h>
#include <logMe/dtMacros.h>

namespace dtOO {
class renderInterface;
class dtTransformer;

class vec2dTwoD : public vec2dFunction {
public:
  dt__class(vec2dTwoD, analyticFunction);
  vec2dTwoD();
  vec2dTwoD(const vec2dTwoD &orig);
  virtual ~vec2dTwoD();
  virtual vec2dTwoD *clone(void) const = 0;
  virtual vec2dTwoD *cloneTransformed(dtTransformer const *const dtT) const = 0;
  virtual vec2dTwoD *create(void) const = 0;
  virtual aFY Y(aFX const &xx) const = 0;
  virtual aFY Y(dtReal const &x0, dtReal const &x1) const;
  virtual dtInt xDim(void) const;
  void setMax(int const &dir, dtReal const &max);
  void setMin(int const &dir, dtReal const &min);
  virtual dtReal xMin(dtInt const &dir) const;
  virtual dtReal xMax(dtInt const &dir) const;
  virtual std::vector<dtVector2> DYdtVector2(aFX const &xx) const;
  std::vector<dtVector2> DYdtVector2(dtReal const &x0, dtReal const &x1) const;
  using analyticFunction::percent_x;
  using analyticFunction::x_percent;
  aFX x_percent(dtReal const &x0, dtReal const &x1) const;
  aFX percent_x(dtReal const &x0, dtReal const &x1) const;
  dtPoint2 YdtPoint2(aFX const &xx) const;
  dtPoint2 YdtPoint2(dtReal const &x0, dtReal const &x1) const;
  dtPoint2 YdtPoint2Percent(aFX const &xx) const;
  dtPoint2 YdtPoint2Percent(dtReal const &x0, dtReal const &x1) const;
  vectorHandling<renderInterface *> getRender(void) const;

private:
  dtReal _min[2];
  dtReal _max[2];
};
dt__H_addCloneForpVH(vec2dTwoD);
} // namespace dtOO
#endif /* VEC2DTWOD_H */
