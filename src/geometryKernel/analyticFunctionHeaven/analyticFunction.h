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

#ifndef analyticFunction_H
#define analyticFunction_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <interfaceHeaven/labelHandling.h>
#include <interfaceHeaven/optionHandling.h>
#include <interfaceHeaven/renderInterface.h>
#include <logMe/dtMacros.h>
#include <vector>

#include "aFX.h"
#include "aFY.h"

#include <mainConceptFwd.h>

namespace dtOO {
class dtTransformer;

//! Abstract base class for analytic mappings between vector spaces.
/*!
 * An analytic function maps an input vector of type aFX to an output vector
 * of type aFY. Derived classes define the input and output dimensions, input
 * bounds, evaluation, cloning, and transformation behavior.
 *
 * Input percent coordinates are normalized to [0, 1] in each direction.
 * Convenience overloads evaluate one-, two-, and three-dimensional functions
 * and return their output components as standard vectors.
 */
class analyticFunction : public labelHandling, public renderInterface {
public:
  dt__class(analyticFunction, labelHandling);
  analyticFunction();
  analyticFunction(const analyticFunction &orig);
  virtual ~analyticFunction();
  //! Create a deep copy of this function.
  virtual analyticFunction *clone(void) const = 0;
  //! Create a copy that may share immutable internal data with this function.
  virtual analyticFunction *weakClone(void) const;
  //! Create a copy transformed by the supplied transformer.
  virtual analyticFunction *cloneTransformed(dtTransformer const *const dtT
  ) const = 0;
  //! Create a default instance of the concrete function type.
  virtual analyticFunction *create(void) const = 0;
  //! Write the label, dynamic class name, and textual representation to the
  //! log.
  virtual void dump(void) const;
  //! Return a textual representation of this function.
  virtual std::string dumpToString(void) const;
  /*!
   * Evaluate the function.
   *
   * @param xx Input vector in native coordinates.
   * @return Function value.
   */
  virtual aFY Y(aFX const &xx) const = 0;
  //! Return the input-space dimension.
  virtual dtInt xDim(void) const = 0;
  //! Return the output-space dimension.
  virtual dtInt yDim(void) const = 0;
  //! Return the lower bound of an input direction.
  virtual dtReal xMin(dtInt const &dir) const = 0;
  //! Return the upper bound of an input direction.
  virtual dtReal xMax(dtInt const &dir) const = 0;
  //! Return the lower bounds of all input directions.
  virtual aFX xMin(void) const;
  //! Return the upper bounds of all input directions.
  virtual aFX xMax(void) const;
  /*!
   * Convert normalized input coordinates to native coordinates.
   *
   * @param xx One percent coordinate in [0, 1] per input direction.
   * @return Native input coordinates.
   */
  virtual aFX x_percent(aFX const &xx) const;
  /*!
   * Convert native input coordinates to normalized coordinates.
   *
   * @param xx Native input coordinates.
   * @return One percent coordinate in [0, 1] per input direction.
   */
  virtual aFX percent_x(aFX const &xx) const;
  /*!
   * Numerically invert the function for an output value.
   *
   * @param yy Target output vector.
   * @return Native input coordinates that produce the target output.
   * @throws Exception if the numerical minimization does not converge.
   */
  virtual aFX invY(aFY const &yy) const;
  //! Return whether this function consists of multiple component functions.
  virtual bool isCompound(void) const;
  //! Return the component functions of a compound function.
  virtual vectorHandling<analyticFunction *> const &vecRef(void) const;
  //! Return the mapping between compound-function and component indices.
  virtual std::map<int, dtInt> const &mapRef(void) const;
  //! Return whether this function represents a transformed function.
  virtual bool isTransformed(void) const;
  //! Return lower and upper output bounds, if provided by the implementation.
  virtual std::pair<aFY, aFY> yBoundingBox(void) const;
  //! Create an empty input vector.
  static aFX aFXZeroD(void);
  //! Create a one-dimensional input vector.
  static aFX aFXOneD(dtReal const &x0);
  //! Create a two-dimensional input vector.
  static aFX aFXTwoD(dtReal const &x0, dtReal const &x1);
  //! Create a three-dimensional input vector.
  static aFX aFXThreeD(dtReal const &x0, dtReal const &x1, dtReal const &x2);
  //! Convert a 2D point to a two-dimensional input vector.
  static aFX aFXTwoD(dtPoint2 const &pp);
  //! Convert a 3D point to a three-dimensional input vector.
  static aFX aFXThreeD(dtPoint3 const &pp);
  //! Create an empty output vector.
  static aFY aFYZeroD(void);
  //! Create a one-dimensional output vector.
  static aFY aFYOneD(dtReal const &x0);
  //! Create a two-dimensional output vector.
  static aFY aFYTwoD(dtReal const &x0, dtReal const &x1);
  //! Create a three-dimensional output vector.
  static aFY aFYThreeD(dtReal const &x0, dtReal const &x1, dtReal const &x2);
  //! Convert a 2D point to a two-dimensional output vector.
  static aFY aFYTwoD(dtPoint2 const &pp);
  //! Convert a 3D point to a three-dimensional output vector.
  static aFY aFYThreeD(dtPoint3 const &pp);
  //! Convert an input vector to an output vector with identical components.
  static aFY aFY_aFX(aFX const &xx);
  //! Convert an output vector to an input vector with identical components.
  static aFX aFX_aFY(aFY const &yy);
  //! Return the Euclidean distance between equally sized input vectors.
  static dtReal distance(aFX const &x0, aFX const &x1);
  /*!
   * Evaluate a one-dimensional function.
   *
   * @throws Exception if xDim() is not one.
   */
  std::vector<dtReal> const operator()(dtReal const &xx) const;
  /*!
   * Evaluate a two-dimensional function.
   *
   * @throws Exception if xDim() is not two.
   */
  std::vector<dtReal> const
  operator()(dtReal const &xx, dtReal const &yy) const;
  /*!
   * Evaluate a three-dimensional function.
   *
   * @throws Exception if xDim() is not three.
   */
  std::vector<dtReal> const
  operator()(dtReal const &xx, dtReal const &yy, dtReal const &zz) const;
};
dt__H_addCloneForpVH(analyticFunction);
} // namespace dtOO
#endif /* analyticFunction_H */
