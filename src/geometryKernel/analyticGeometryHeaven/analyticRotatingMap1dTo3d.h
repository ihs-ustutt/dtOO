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

#ifndef analyticRotatingMap1dTo3d_H
#define analyticRotatingMap1dTo3d_H

#include <dtOOTypeDef.h>

#define _USE_MATH_DEFINES
#include "analyticCurve.h"
#include "analyticSurface.h"
#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <math.h>
#include <vector>

namespace dtOO {
class analyticCurve;

//! CAD surface of revolution with helpers for cylindrical coordinate
//! conversions.
class analyticRotatingMap1dTo3d : public analyticSurface {
public:
  dt__class(analyticRotatingMap1dTo3d, analyticGeometry);
  analyticRotatingMap1dTo3d();
  analyticRotatingMap1dTo3d(
    dtSurface const &dtS,
    dtPoint3 const &pp,
    dtVector3 const &vv,
    dtReal const &angle = 2. * M_PI
  );
  analyticRotatingMap1dTo3d(
    dtSurface const &dtS, dtVector3 const &vv, dtReal const &angle = 2. * M_PI
  );
  analyticRotatingMap1dTo3d(analyticRotatingMap1dTo3d const &orig);
  virtual analyticRotatingMap1dTo3d *clone(void) const;
  virtual analyticRotatingMap1dTo3d *create(void) const;
  virtual ~analyticRotatingMap1dTo3d();
  //! Return a point on the rotation axis.
  dtPoint3 getOrigin(void) const;
  //! Return the normalized rotation-axis vector.
  dtVector3 getRotVector(void) const;
  //! Return the revolution angle in radians.
  dtReal getAngle(void) const;
  //! Convert an azimuth angle in radians to native u.
  virtual dtReal u_phi(dtReal const &arg) const;
  //! Convert native u to an azimuth angle in radians.
  virtual dtReal phi_u(dtReal const &arg) const;
  //! Convert meridional arc length to native v.
  virtual dtReal v_m(dtReal const &arg) const;
  //! Convert native v to meridional arc length.
  virtual dtReal m_v(dtReal const &arg) const;
  //! Evaluate the surface from azimuth angle and axis coordinate.
  virtual dtPoint3 xyz_phiZ(dtReal const &arg0, dtReal const &arg1) const;
  //! Convert azimuth angle and axis coordinate to native UV.
  virtual dtPoint2 uv_phiZ(dtReal const &arg0, dtReal const &arg1) const;
  //! Offset base UV by circumferential and meridional distances.
  virtual dtPoint2 uv_deltaPhiRadiusDeltaM(
    dtReal const &bU, dtReal const &bV, dtReal const &arg0, dtReal const &arg1
  ) const;
  //! Convert circumferential distance and meridional arc length to native UV.
  virtual dtPoint2 uv_phiRadiusM(dtReal const &arg0, dtReal const &arg1) const;
  //! Convert azimuth angle and meridional arc length to native UV.
  virtual dtPoint2 uv_phiM(dtReal const &arg0, dtReal const &arg1) const;
  //! Return the axial coordinate at the minimum native v.
  virtual dtReal zMin(void) const;
  //! Return the axial coordinate at the maximum native v.
  virtual dtReal zMax(void) const;
  //! Convert native v to distance along the rotation axis.
  virtual dtReal z_v(dtReal const &arg) const;
  //! Convert distance along the rotation axis to native v.
  virtual dtReal v_z(dtReal const &arg) const;
  //! Return the revolution radius at native v.
  virtual dtReal r_v(dtReal const &arg0) const;
  //! Return the revolution radius at meridional arc length.
  virtual dtReal r_m(dtReal const &arg0) const;
  //! Move the stored origin onto the rotation axis of the generating surface.
  void correctOrigin(void);
  virtual map2dTo3d *
  segmentRectangle(dtPoint2 const &p0, dtPoint2 const &p1) const;

private:
  dtCurve const *ptrConstRadiusCurve(void) const;

private:
  dtPoint3 _pp;
  dtVector3 _vv;
  dtReal _angle;
};
} // namespace dtOO
#endif /* analyticRotatingMap1dTo3d_H */
