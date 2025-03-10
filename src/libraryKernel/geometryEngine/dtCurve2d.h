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

#ifndef DTCURVE2D_H
#define DTCURVE2D_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>

namespace dtOO {
class dtCurve2d {
public:
  dt__classOnlyName(dtCurve2d);
  //
  // overload
  //
  dtCurve2d();
  virtual ~dtCurve2d();
  virtual dtCurve2d *clone(void) const = 0;
  virtual dtReal minPara(dtInt const &dir) const = 0;
  virtual dtReal maxPara(dtInt const &dir) const = 0;
  virtual dtInt order(void) const = 0;
  virtual bool closed(void) const = 0;
  virtual dtPoint2 point(dtReal const uu) const = 0;
  virtual dtVector2 firstDer(dtReal const uu) const = 0;
  virtual dtInt nControlPoints(void) const = 0;
  virtual dtPoint2 controlPoint(dtInt const nPoint) const = 0;
  virtual dtReal l_u(dtReal const uu) const = 0;
  virtual dtReal u_l(dtReal const length) const = 0;
  virtual dtReal reparam(dtPoint2 const point) const = 0;
  virtual void translate(dtVector2 const &tt) = 0;
  virtual dtVector2 normal(dtReal const &uu) const = 0;
  //
  // optional overload
  //
  void dump(void);
  virtual std::string dumpToString(void) const;
  //
  //
  //
  dtReal minU(void) const;
  dtReal maxU(void) const;
  dtReal u_uPercent(dtReal const percent) const;
  dtReal uPercent_u(dtReal const uu) const;
  dtPoint2 pointPercent(dtReal const percent) const;
  dtVector2 normalPercent(dtReal const percent) const;
  dtPoint2 lastControlPoint(void) const;
  dtReal length(void) const;
  dtReal l_uPercent(dtReal const percent) const;
  dtReal u_lPercent(dtReal const percent) const;
  virtual void setControlPoint(dtInt const nPoint, dtPoint2 const point);
};
dt__H_addCloneForpVH(dtCurve2d);
} // namespace dtOO
#endif /* DTCURVE2D_H */
