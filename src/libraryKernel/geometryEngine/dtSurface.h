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

#ifndef DTSURFACE_H
#define DTSURFACE_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>

namespace dtOO {
class dtCurve;
class dtCurve2d;

class dtSurface {
public:
  dt__classOnlyName(dtSurface);
  dtSurface();
  virtual ~dtSurface();
  //
  // overload
  //
  virtual dtSurface *clone(void) const = 0;
  virtual dtReal minPara(dtInt const dim) const = 0;
  virtual dtReal maxPara(dtInt const dim) const = 0;
  virtual bool closed(dtInt const dim) const = 0;
  virtual dtPoint3 point(dtReal const uu, dtReal const vv) const = 0;
  virtual dtVector3 normal(dtReal const uu, dtReal const vv) const = 0;
  virtual std::vector<dtVector3>
  firstDer(dtReal const uu, dtReal const vv) const = 0;
  virtual std::vector<dtVector3>
  secondDer(dtReal const uu, dtReal const vv) const = 0;
  virtual dtCurve *segmentConstU(
    dtReal const uu, dtReal const vvMin, dtReal const vvMax
  ) const = 0;
  virtual dtCurve *segmentConstV(
    dtReal const vv, dtReal const uuMin, dtReal const uuMax
  ) const = 0;
  //
  // optional overload
  //
  virtual dtInt continuity(void) const;
  virtual dtPoint2 reparam(dtPoint3 const point) const;
  virtual dtPoint3 controlPoint(dtInt const uI, dtInt const vI) const;
  virtual void
  setControlPoint(dtInt const uI, dtInt const vI, dtPoint3 const point);
  virtual dtInt nControlPoints(dtInt const dim) const;
  void dump(void) const;
  virtual std::string dumpToString(void) const;
  virtual void offsetNormal(dtReal const nn);
  //
  //
  //
  dtReal minU(void) const;
  dtReal maxU(void) const;
  dtReal minV(void) const;
  dtReal maxV(void) const;
  bool closedU(void) const;
  bool closedV(void) const;
  dtReal u_uPercent(dtReal const percent) const;
  dtReal uPercent_u(dtReal const uu) const;
  dtReal v_vPercent(dtReal const percent) const;
  dtReal vPercent_v(dtReal const vv) const;
  dtPoint3 point(dtPoint2 const &pp) const;
  dtPoint3 pointPercent(dtPoint2 const &ppP) const;
  dtPoint3 pointPercent(dtReal const &uP, dtReal const &vP) const;
  dtPoint3 getPoint3d(dtPoint2 const &pUV) const;
  dtPoint3 getPointPercent3d(dtReal const uP, dtReal const vP) const;
  std::vector<dtPoint3> getPoint3d(
    std::vector<dtReal> const &uu, std::vector<dtReal> const &vv
  ) const;
  std::vector<dtPoint3> getPointPercent3d(
    std::vector<dtReal> const &uu, std::vector<dtReal> const &vv
  ) const;
  std::vector<dtPoint3> getPoint3d(
    std::vector<dtReal> const *const uuV, std::vector<dtReal> const *const vvV
  ) const;
  dtVector3 firstDerU(dtReal const uu, dtReal const vv) const;
  dtVector3 firstDerV(dtReal const uu, dtReal const vv) const;
  dtVector3 secondDerUU(dtReal const uu, dtReal const vv) const;
  dtVector3 secondDerVV(dtReal const uu, dtReal const vv) const;
  dtVector3 secondDerUV(dtReal const uu, dtReal const vv) const;
  dtVector3 normalPercent(dtReal const uP, dtReal const vP) const;
  dtPoint2 uv_uvPercent(dtPoint2 const uvPercent) const;
  dtInt nControlPointsU(void) const;
  dtInt nControlPointsV(void) const;
  dtCurve *segmentConstU(dtReal const uu) const;
  dtCurve *segmentConstV(dtReal const vv) const;
  dtCurve *segmentConstUPercent(dtReal const uu) const;
  dtCurve *segmentConstVPercent(dtReal const vv) const;
  dtCurve *segmentConstUPercent(
    dtReal const uu, dtReal const vvMin, dtReal const vvMax
  ) const;
  dtCurve *segmentConstVPercent(
    dtReal const vv, dtReal const uuMin, dtReal const uuMax
  ) const;
  dtCurve *segmentConst(
    dtInt const &dir, dtReal const at, dtReal const from, dtReal const to
  ) const;
  dtCurve *segmentConst(dtInt const &dir, dtReal const at) const;
  dtCurve *segmentConstPercent(dtInt const &dir, dtReal const at) const;
  dtCurve *segmentConstPercent(
    dtInt const &dir, dtReal const at, dtReal const from, dtReal const to
  ) const;
  dtPoint2 reparamPercent(dtPoint3 const point) const;
  static bool inXYZTolerance(dtPoint3 const &p0, dtPoint3 const &p1);
};
dt__H_addCloneForpVH(dtSurface);
} // namespace dtOO
#endif /* DTSURFACE_H */
