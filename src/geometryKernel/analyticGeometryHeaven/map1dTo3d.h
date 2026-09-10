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

#ifndef MAP1DTO3D_H
#define MAP1DTO3D_H

#include <dtOOTypeDef.h>

#include "analyticGeometry.h"
#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>

namespace dtOO {
class dtTransformer;

//! Abstract one-dimensional map embedded in three-dimensional space.
/*!
 * This class represents a parameterized curve p(u). Concrete implementations
 * provide point evaluation, creation, cloning, and transformation. Native
 * parameter values lie in [getUMin(), getUMax()], while percent variants use
 * the normalized interval [0, 1].
 *
 * The optional operations provide numerical derivatives, curve segments,
 * arc-length conversions, rendering, and closest-point reparameterization.
 */
class map1dTo3d : public analyticGeometry {
public:
  dt__class(map1dTo3d, analyticGeometry);
  map1dTo3d();
  map1dTo3d(const map1dTo3d &orig);
  virtual ~map1dTo3d();
  //! Return the one-dimensional parameter-space dimension.
  virtual dtInt dim(void) const;
  //! Evaluate the curve from a one-element parameter array.
  virtual dtPoint3 getPoint(dtReal const *const uvw) const;
  //! Evaluate the first derivative from a one-element parameter array.
  virtual ::std::vector<dtVector3> firstDer(dtReal const *const uvw) const;
  //
  // overload
  //
  //! Create a default instance of the concrete curve type.
  virtual map1dTo3d *create(void) const = 0;
  //! Create a copy of this curve.
  virtual map1dTo3d *clone(void) const = 0;
  //! Create a copy transformed by the supplied transformer.
  virtual map1dTo3d *cloneTransformed(dtTransformer const *const dtT) const = 0;
  //! Evaluate the curve at a native u coordinate.
  virtual dtPoint3 getPoint(dtReal const &uu) const = 0;
  //
  // optional overload
  //
  //! Return the first derivative with respect to u.
  virtual dtVector3 firstDerU(dtReal const &uu) const;
  //! Create the curve segment between two native u coordinates.
  virtual map1dTo3d *segment(dtReal const &u0, dtReal const &u1) const;
  //! Return arc length from getUMin() to a native u coordinate.
  virtual dtReal l_u(dtReal const &uu) const;
  //! Return the native u coordinate at an arc length from getUMin().
  virtual dtReal u_l(dtReal const &ll) const;
  //! Return renderable representations of this curve.
  virtual vectorHandling<renderInterface *> getRender(void) const;
  //! Return the native u coordinate closest to a spatial point.
  virtual dtReal reparamOnEdge(dtPoint3 const &ppXYZ) const;
  //
  //
  //
  //! Evaluate the curve at a normalized u coordinate.
  dtPoint3 getPointPercent(dtReal const &uu) const;
  //! Return the first derivative at a normalized u coordinate.
  dtVector3 firstDerUPercent(dtReal const &uP) const;
  //! Create the curve segment between two normalized u coordinates.
  map1dTo3d *segmentPercent(dtReal const &u0, dtReal const &u1) const;
  dtInt getRenderResolutionU(void) const;
  dtReal reparamPercentOnEdge(dtPoint3 const &ppXYZ) const;
  //! Convert normalized u to its native parameter value.
  dtReal u_percent(dtReal const &uu) const;
  //! Return native u at a normalized arc length.
  dtReal u_lPercent(dtReal const &lP) const;
  //! Convert native u to a normalized parameter value.
  dtReal percent_u(dtReal const &uu) const;
  //! Convert arc length to a fraction of total curve length.
  dtReal percent_l(dtReal const &ll) const;
  //! Return normalized arc length at a native u coordinate.
  dtReal lPercent_u(dtReal const &uu) const;
  bool isClosedU(void) const;
  dtReal getUMin(void) const;
  dtReal getUMax(void) const;
  //! Return the total curve length.
  dtReal length(void) const;
  //! Return native u for a normalized u coordinate.
  dtReal operator%(const dtReal &percent) const;
  //! Return native u for an arc length.
  dtReal operator|(const dtReal &length) const;
  //! Return native u for a normalized arc length.
  dtReal operator&(const dtReal &lengthPercent) const;

private:
  dtReal l_u(dtReal const &uu, dtInt const &nP) const;
  double funValue(const double xx) const;

private:
  mutable dtReal _tmpL;
  static dtReal _deltaPer;
};

//
// boost requires this method in ptr_vector
//
map1dTo3d *new_clone(map1dTo3d const &m1d);
} // namespace dtOO
#endif /* MAP1DTO3D_H */
