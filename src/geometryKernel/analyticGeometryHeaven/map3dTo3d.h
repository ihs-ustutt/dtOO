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

#ifndef MAP3DTO3D_H
#define MAP3DTO3D_H

#include <dtOOTypeDef.h>

#include "analyticGeometry.h"
#include <dtLinearAlgebra.h>
#include <interfaceHeaven/twoDArrayHandling.h>
#include <logMe/dtMacros.h>

namespace dtOO {
class map1dTo3d;
class map2dTo3d;
class dtTransformer;

//! Abstract three-dimensional map embedded in three-dimensional space.
/*!
 * This class represents a parameterized volume p(u, v, w). Concrete
 * implementations provide native-coordinate point evaluation, creation,
 * cloning, and transformation. Percent-coordinate variants use normalized
 * coordinates in [0, 1] for every parameter direction.
 *
 * Common helpers expose first derivatives, volume reparameterization,
 * boundary surfaces, curve and surface segments, parameter bounds, and
 * rendering resolutions.
 */
class map3dTo3d : public analyticGeometry {
public:
  dt__class(map3dTo3d, analyticGeometry);
  map3dTo3d();
  map3dTo3d(const map3dTo3d &orig);
  virtual ~map3dTo3d();
  //! Return the three-dimensional parameter-space dimension.
  virtual dtInt dim(void) const;
  //! Evaluate the volume from a three-element parameter array.
  dtPoint3 getPoint(dtReal const *const uvw) const;
  //! Evaluate first derivatives from a three-element parameter array.
  virtual ::std::vector<dtVector3> firstDer(dtReal const *const uvw) const;
  //! Return renderable representations of this volume.
  virtual vectorHandling<renderInterface *> getRender(void) const;
  //
  // overload
  //
  //! Create a default instance of the concrete volume type.
  virtual map3dTo3d *create(void) const = 0;
  //! Create a copy of this volume.
  virtual map3dTo3d *clone(void) const = 0;
  //! Create a copy transformed by the supplied transformer.
  virtual map3dTo3d *cloneTransformed(dtTransformer const *const dtT) const = 0;
  //! Evaluate the volume at native u, v, and w coordinates.
  virtual dtPoint3
  getPoint(dtReal const &uu, dtReal const &vv, dtReal const &ww) const = 0;
  //
  // optional overload
  //
  //! Return native volume coordinates closest to a spatial point.
  virtual dtPoint3 reparamInVolume(dtPoint3 const &ppXYZ) const;
  //! Return first derivatives at native u, v, and w coordinates.
  virtual std::vector<dtVector3>
  firstDer(dtReal const &uu, dtReal const &vv, dtReal const &ww) const;
  //! Create a curve segment between two native volume coordinates.
  virtual map1dTo3d *segment(dtPoint3 const &p0, dtPoint3 const &p1) const;
  //! Create a surface segment from a native-coordinate point array.
  virtual map2dTo3d *segment(twoDArrayHandling<dtPoint3> const &pp) const;
  //! Return the native constant-u boundary surface.
  virtual map2dTo3d *segmentConstU(dtReal const &uu) const;
  //! Return the native constant-v boundary surface.
  virtual map2dTo3d *segmentConstV(dtReal const &vv) const;
  //! Return the native constant-w boundary surface.
  virtual map2dTo3d *segmentConstW(dtReal const &ww) const;
  virtual map2dTo3d *
  segmentConstU(dtReal const &uu, dtPoint2 const &p0, dtPoint2 const &p1) const;
  virtual map2dTo3d *
  segmentConstV(dtReal const &vv, dtPoint2 const &p0, dtPoint2 const &p1) const;
  virtual map2dTo3d *
  segmentConstW(dtReal const &ww, dtPoint2 const &p0, dtPoint2 const &p1) const;
  //
  //
  //
  dtVector3
  firstDerU(dtReal const &uu, dtReal const &vv, dtReal const &ww) const;
  dtVector3
  firstDerV(dtReal const &uu, dtReal const &vv, dtReal const &ww) const;
  dtVector3
  firstDerW(dtReal const &uu, dtReal const &vv, dtReal const &ww) const;
  //! Return normalized volume coordinates closest to a spatial point.
  dtPoint3 reparamPercentInVolume(dtPoint3 const &ppXYZ) const;
  //! Return approximate normalized volume coordinates for a spatial point.
  dtPoint3 approxPercentInVolume(dtPoint3 const &ppXYZ) const;
  bool isClosedU(void) const;
  bool isClosedV(void) const;
  bool isClosedW(void) const;
  dtReal getUMin(void) const;
  dtReal getUMax(void) const;
  dtReal getVMin(void) const;
  dtReal getVMax(void) const;
  dtReal getWMin(void) const;
  dtReal getWMax(void) const;
  //! Return whether native volume coordinates lie within the parameter bounds.
  bool inRange(dtPoint3 const &pUVW) const;
  //! Evaluate the volume at normalized u, v, and w coordinates.
  dtPoint3
  getPointPercent(dtReal const &uu, dtReal const &vv, dtReal const &ww) const;
  dtPoint3 getPoint(dtPoint3 const &ppUVW) const;
  dtPoint3 getPointPercent(dtPoint3 const &ppUVW) const;
  dtReal u_percent(dtReal const &uu) const;
  dtReal v_percent(dtReal const &vv) const;
  dtReal w_percent(dtReal const &ww) const;
  dtReal percent_u(dtReal const &uu) const;
  dtReal percent_v(dtReal const &vv) const;
  dtReal percent_w(dtReal const &ww) const;
  //! Convert native u, v, and w coordinates to normalized coordinates.
  dtPoint3 percent_uvw(dtPoint3 const &pUVW) const;
  //! Convert normalized u, v, and w coordinates to native coordinates.
  dtPoint3 uvw_percent(dtPoint3 const &pp) const;
  dtInt getRenderResolutionU(void) const;
  dtInt getRenderResolutionV(void) const;
  dtInt getRenderResolutionW(void) const;
  map2dTo3d *segmentConstUPercent(
    dtReal const &uu, dtPoint2 const &p0, dtPoint2 const &p1
  ) const;
  map2dTo3d *segmentConstVPercent(
    dtReal const &vv, dtPoint2 const &p0, dtPoint2 const &p1
  ) const;
  map2dTo3d *segmentConstWPercent(
    dtReal const &ww, dtPoint2 const &p0, dtPoint2 const &p1
  ) const;
  map2dTo3d *segmentConstUPercent(dtReal const &uu) const;
  map2dTo3d *segmentConstVPercent(dtReal const &vv) const;
  map2dTo3d *segmentConstWPercent(dtReal const &ww) const;
  map2dTo3d *segment(
    dtPoint3 const &p0,
    dtPoint3 const &p1,
    dtPoint3 const &p2,
    dtPoint3 const &p3
  ) const;
  map2dTo3d *segmentPercent(twoDArrayHandling<dtPoint3> const &pp) const;
  map1dTo3d *segmentPercent(dtPoint3 const &p0, dtPoint3 const &p1) const;
  map2dTo3d *segmentPercent(
    dtPoint3 const &p0,
    dtPoint3 const &p1,
    dtPoint3 const &p2,
    dtPoint3 const &p3
  ) const;

private:
  static dtReal _deltaPer;
};
dt__H_addCloneForpVH(map3dTo3d);
} // namespace dtOO
#endif /* MAP3DTO3D_H */
