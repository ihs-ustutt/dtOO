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

#ifndef analyticGeometry_H
#define analyticGeometry_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <interfaceHeaven/labelHandling.h>
#include <interfaceHeaven/labeledVectorHandling.h>
#include <interfaceHeaven/renderInterface.h>
#include <interfaceHeaven/vectorHandling.h>
#include <logMe/dtMacros.h>
#include <mainConceptFwd.h>
#include <string>
#include <vector>

namespace dtOO {
//! Abstract base class for parameterized analytic geometries.
/*!
 * Derived classes represent curves, surfaces, or volumes embedded in
 * three-dimensional space. They define the parameter-space dimension and
 * bounds and evaluate points and first derivatives in native coordinates.
 *
 * Percent-coordinate helpers map each parameter from [0, 1] to its native
 * range. Bounding boxes and their length, area, or volume measures are
 * calculated lazily from a regular parameter-space sample grid.
 */
class analyticGeometry : public labelHandling, public renderInterface {
public:
  dt__class(analyticGeometry, labelHandling);
  analyticGeometry();
  virtual ~analyticGeometry();
  analyticGeometry(analyticGeometry const &orig);
  /*!
   * Evaluate a point using percent coordinates.
   *
   * @param uvw One normalized coordinate per parameter direction, in [0, 1].
   * @return Point on the geometry.
   */
  dtPoint3 getPointPercent(dtReal const *const uvw) const;
  /*!
   * Evaluate first derivatives using percent coordinates.
   *
   * @param uvw One normalized coordinate per parameter direction, in [0, 1].
   * @return First derivative vector for every parameter direction.
   */
  ::std::vector<dtVector3> firstDerPercent(dtReal const *const uvw) const;
  //
  // overload
  //
  //! Create a copy of this geometry.
  virtual analyticGeometry *clone(void) const = 0;
  //! Create a default instance of the concrete geometry type.
  virtual analyticGeometry *create(void) const = 0;
  //! Return the parameter-space dimension (one, two, or three).
  virtual dtInt dim(void) const = 0;
  //! Return whether the parameter direction is periodic or closed.
  virtual bool isClosed(dtInt const &dir) const = 0;
  //! Return the lower bound of a parameter direction.
  virtual dtReal getMin(dtInt const &dir) const = 0;
  //! Return the upper bound of a parameter direction.
  virtual dtReal getMax(dtInt const &dir) const = 0;
  /*!
   * Evaluate a point using native parameter coordinates.
   *
   * @param uvw One coordinate per parameter direction.
   * @return Point on the geometry.
   */
  virtual dtPoint3 getPoint(dtReal const *const uvw) const = 0;
  /*!
   * Evaluate first derivatives using native parameter coordinates.
   *
   * @param uvw One coordinate per parameter direction.
   * @return First derivative vector for every parameter direction.
   */
  virtual ::std::vector<dtVector3> firstDer(dtReal const *const uvw) const = 0;
  //
  // optional overload
  //
  //! Return a textual representation of this geometry.
  virtual std::string dumpToString(void) const;
  //! Return whether this geometry represents a transformed object.
  virtual bool isTransformed(void) const;
  //! Return whether this geometry consists of multiple sub-geometries.
  virtual bool isCompound(void) const;
  //! Return the constituent geometries of a compound geometry.
  virtual vectorHandling<analyticGeometry const *> compoundInternal(void) const;
  //! Return the lazily computed axis-aligned bounding box.
  std::pair<dtPoint3, dtPoint3> boundingBox(void) const;
  /*!
   * Return the sampled geometric measure of the bounding-box grid.
   *
   * The measure is length for curves, area for surfaces, and volume for
   * volumes.
   */
  dtReal boundingBoxValue(void) const;
  //! Recompute the cached bounding box and its sampled geometric measure.
  void updateBoundingBox(void) const;
  //! Return the diagonal length of the lazily computed bounding box.
  dtReal characteristicLength(void) const;
  //! Return the points at all corners of the normalized parameter domain.
  std::vector<dtPoint3> cornerPoints(void) const;
  /*!
   * Find native parameter coordinates for a point on the geometry.
   *
   * @param xyz Point to reparameterize.
   * @param uvw Receives one native coordinate per parameter direction.
   * @return True when reparameterization succeeds.
   */
  virtual bool reparam(dtPoint3 const &xyz, std::vector<dtReal> &uvw) const;
  //
  //
  //
  //! Evaluate a point using native coordinates supplied as a vector.
  dtPoint3 getPoint(std::vector<dtReal> const &uvw) const;
  /*!
   * Compare two points using the configured XYZ tolerance.
   *
   * @param p0 First point.
   * @param p1 Second point.
   * @param dist Receives the Euclidean distance between the points.
   * @param output Emit a warning when the points are outside the tolerance.
   * @param inc Multiplier applied to the configured tolerance.
   * @return True if the points are within tolerance.
   */
  static bool inXYZTolerance(
    dtPoint3 const &p0,
    dtPoint3 const &p1,
    dtReal *const dist,
    bool output,
    dtReal inc
  );
  //! Return the configured XYZ tolerance.
  static dtReal XYZTolerance(void);
  //! Return whether two points are within the configured XYZ tolerance.
  static bool inXYZTolerance(dtPoint3 const &p0, dtPoint3 const &p1);
  //! Return whether a distance is within the configured XYZ tolerance.
  static bool inXYZTolerance(dtReal const dist, dtReal inc = 1.);
  //! Return whether the sampled geometric measure is within XYZ tolerance.
  bool degenerated(void) const;
  //! Return whether two 3D parameter coordinates are within UVW tolerance.
  static bool inUVWTolerance(dtPoint3 const &p0, dtPoint3 const &p1);
  //! Return whether two 2D parameter coordinates are within UVW tolerance.
  static bool inUVWTolerance(dtPoint2 const &p0, dtPoint2 const &p1);
  //! Return whether two scalar parameter coordinates are within UVW tolerance.
  static bool inUVWTolerance(dtReal const &p0, dtReal const &p1);
  //
  //
  //
  /*!
   * Compare geometries by matching their normalized parameter-domain corners.
   *
   * @return True if both geometries have the same dimension and equivalent
   *         corner points within XYZ tolerance.
   */
  bool equal(const analyticGeometry &other) const;
  //! Pointer overload of equal(analyticGeometry const &).
  bool equal(analyticGeometry const *const other) const;
  //
  //
  //
  //! Write the label, dynamic class name, and textual representation to the
  //! log.
  void dump(void) const;
  /*!
   * Set the rendering resolution for a parameter direction.
   *
   * @param dir Parameter direction: 0 (u), 1 (v), or 2 (w).
   * @param value Number of rendering samples in that direction.
   */
  void setRenderResolution(int const &dir, dtInt const &value) const;
  /*!
   * Return the rendering resolution for a parameter direction.
   *
   * @param dir Parameter direction: 0 (u), 1 (v), or 2 (w).
   */
  dtInt getRenderResolution(int const &dir) const;
  /*!
   * Convert a normalized parameter coordinate to its native value.
   *
   * @param per Normalized coordinate in [0, 1].
   * @param dir Parameter direction: 0 (u), 1 (v), or 2 (w).
   */
  dtReal val_percent(dtReal const &per, int const &dir) const;
  /*!
   * Convert a native parameter coordinate to its normalized value.
   *
   * @param val Native parameter coordinate.
   * @param dir Parameter direction: 0 (u), 1 (v), or 2 (w).
   */
  dtReal percent_val(dtReal const &val, int const &dir) const;

private:
  mutable dtInt _resU;
  mutable dtInt _resV;
  mutable dtInt _resW;
  mutable dtReal _characteristicLength;
  mutable std::pair<dtPoint3, dtPoint3> _boundingBox;
  mutable dtReal _boundingBoxValue;
};
dt__H_addCloneForpVH(analyticGeometry);
} // namespace dtOO
#endif /* analyticGeometry_H */
