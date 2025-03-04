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

#ifndef VEC2DBILINEARTWOD_H
#define VEC2DBILINEARTWOD_H

#include <dtOOTypeDef.h>

#include "vec2dTwoD.h"
#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <vector>

namespace dtOO {
class dtTransformer;

class vec2dBiLinearTwoD : public vec2dTwoD {
public:
  dt__class(vec2dBiLinearTwoD, analyticFunction);
  vec2dBiLinearTwoD();
  vec2dBiLinearTwoD(vec2dBiLinearTwoD const &orig);
  vec2dBiLinearTwoD(
    dtPoint2 const &p0,
    dtPoint2 const &p1,
    dtPoint2 const &p2,
    dtPoint2 const &p3
  );
  vec2dBiLinearTwoD(dtPoint2 const &p0, dtPoint2 const &p2);
  vec2dBiLinearTwoD *clone(void) const;
  virtual vec2dBiLinearTwoD *cloneTransformed(dtTransformer const *const dtT
  ) const;
  vec2dBiLinearTwoD *create(void) const;
  virtual ~vec2dBiLinearTwoD();
  virtual aFY Y(aFX const &xx) const;
  virtual bool closed(dtInt const &dir) const;
  aFX invY(aFY const &yy) const;
  bool insideY(aFY const &yy) const;

private:
  static dtReal cross2(
    dtReal const &x0, dtReal const &y0, dtReal const &x1, dtReal const &y1
  );
  static dtInt inverseBilerp(
    dtReal x0,
    dtReal y0,
    dtReal x1,
    dtReal y1,
    dtReal x2,
    dtReal y2,
    dtReal x3,
    dtReal y3,
    dtReal x,
    dtReal y,
    dtReal *sout,
    dtReal *tout,
    dtReal *s2out,
    dtReal *t2out
  );
  static dtInt
  in_range(dtReal val, dtReal range_min, dtReal range_max, dtReal tol);
  static dtInt equals(dtReal a, dtReal b, dtReal tolerance);

private:
  dtVector2 _v0;
  dtVector2 _v1;
  dtVector2 _v2;
  dtVector2 _v3;
  dtPoint2 _p0;
  dtPoint2 _p1;
  dtPoint2 _p2;
  dtPoint2 _p3;
};
} // namespace dtOO
#endif /* VEC2DBILINEARTWOD_H */
