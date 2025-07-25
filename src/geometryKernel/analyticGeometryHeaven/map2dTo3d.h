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

#ifndef MAP2DTO3D_H
#define MAP2DTO3D_H

#include <dtOOTypeDef.h>

#include "analyticGeometry.h"
#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>

namespace dtOO {
class map1dTo3d;
class dtTransformer;

class map2dTo3d : public analyticGeometry {
public:
  dt__class(map2dTo3d, analyticGeometry);
  map2dTo3d();
  map2dTo3d(const map2dTo3d &orig);
  virtual ~map2dTo3d();
  virtual dtInt dim(void) const;
  virtual dtPoint3 getPoint(dtReal const *const uvw) const;
  virtual ::std::vector<dtVector3> firstDer(dtReal const *const uvw) const;
  //
  // overload
  //
  virtual map2dTo3d *create(void) const = 0;
  virtual map2dTo3d *clone(void) const = 0;
  virtual map2dTo3d *cloneTransformed(dtTransformer const *const dtT) const = 0;
  virtual dtPoint3 getPoint(dtReal const &uu, dtReal const &vv) const = 0;
  //
  // optional overload
  //
  virtual vectorHandling<renderInterface *> getRender(void) const;
  virtual vectorHandling<renderInterface *> getExtRender(void) const;
  virtual std::vector<dtVector3> firstDer(dtPoint2 const &pp) const;
  virtual std::vector<dtVector3>
  secondDer(dtReal const &uu, dtReal const &vv) const;
  virtual dtVector3 normal(dtReal const &uu, dtReal const &vv) const;
  virtual dtPoint2 reparamOnFace(dtPoint3 const &ppXYZ) const;
  virtual dtPoint2 approxOnFace(dtPoint3 const &ppXYZ) const;
  virtual map1dTo3d *segment(dtPoint2 const &p0, dtPoint2 const &p1) const;
  virtual map1dTo3d *
  segmentConstU(dtReal const &uu, dtReal const &p0, dtReal const &p1) const;
  virtual map1dTo3d *
  segmentConstV(dtReal const &vv, dtReal const &p0, dtReal const &p1) const;
  virtual map2dTo3d *segment(
    dtPoint2 const &p0,
    dtPoint2 const &p1,
    dtPoint2 const &p2,
    dtPoint2 const &p3
  ) const;
  virtual map2dTo3d *
  segmentRectangle(dtPoint2 const &p0, dtPoint2 const &p1) const;
  //
  //
  //
  dtPoint3 getPoint(dtPoint2 const &pUV) const;
  dtPoint3 getPoint(dtPoint2 const *const pUV) const;
  dtPoint3 getPointPercent(dtReal const &uu, dtReal const &vv) const;
  dtPoint3 getPointPercent(dtPoint2 const &pUV) const;
  dtVector3 getPointPercentVector(dtReal const &uu, dtReal const &vv) const;
  dtMatrix jacobi(dtReal const &uu, dtReal const &vv) const;
  dtMatrix jacobi(dtPoint2 const &pp) const;
  bool isClosedU(void) const;
  bool isClosedV(void) const;
  dtReal getUMin(void) const;
  dtReal getUMax(void) const;
  dtReal getVMin(void) const;
  dtReal getVMax(void) const;
  bool inRange(dtPoint2 const &pUV) const;
  dtVector3 normal(dtPoint2 const &pp) const;
  dtVector3 normalPercent(dtReal const &uu, dtReal const &vv) const;
  std::vector<dtVector3> firstDer(dtReal const &uu, dtReal const &vv) const;
  dtVector3 firstDerU(dtReal const &uu, dtReal const &vv) const;
  dtVector3 firstDerV(dtReal const &uu, dtReal const &vv) const;
  dtVector3 secondDerUU(dtReal const &uu, dtReal const &vv) const;
  dtVector3 secondDerVV(dtReal const &uu, dtReal const &vv) const;
  dtVector3 secondDerUV(dtReal const &uu, dtReal const &vv) const;
  dtPoint2 reparamPercentOnFace(dtPoint3 const &ppXYZ) const;
  dtPoint2 approxPercentOnFace(dtPoint3 const &ppXYZ) const;
  dtInt getRenderResolutionU(void) const;
  dtInt getRenderResolutionV(void) const;
  dtPoint2 uv_percent(dtPoint2 const &pUV) const;
  dtPoint2 uv_percent(dtReal const &uu, dtReal const &vv) const;
  dtReal u_percent(dtReal const &uu) const;
  dtReal v_percent(dtReal const &vv) const;
  dtPoint2 percent_uv(dtPoint2 const &pUV) const;
  dtReal percent_u(dtReal const &uu) const;
  dtReal percent_v(dtReal const &vv) const;
  map1dTo3d *segmentConstU(dtReal const &uu) const;
  map1dTo3d *segmentConstV(dtReal const &vv) const;
  map1dTo3d *segmentConstUPercent(
    dtReal const &uu, dtReal const &p0, dtReal const &p1
  ) const;
  map1dTo3d *segmentConstVPercent(
    dtReal const &vv, dtReal const &p0, dtReal const &p1
  ) const;
  map1dTo3d *segmentConstUPercent(dtReal const &uu) const;
  map1dTo3d *segmentConstVPercent(dtReal const &vv) const;
  map1dTo3d *segmentPercent(dtPoint2 const &p0, dtPoint2 const &p1) const;
  map2dTo3d *segmentPercent(
    dtPoint2 const &p0,
    dtPoint2 const &p1,
    dtPoint2 const &p2,
    dtPoint2 const &p3
  ) const;
  map2dTo3d *
  segmentRectanglePercent(dtPoint2 const &p0, dtPoint2 const &p1) const;
  dtPoint2 operator%(const dtPoint2 &percent) const;

private:
  static dtReal _deltaPer;
};

//
// boost requires this method in ptr_vector
//
map2dTo3d *new_clone(map2dTo3d const &m2d);
} // namespace dtOO
#endif /* MAP2DTO3D_H */
