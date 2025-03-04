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

#ifndef SCAONED_H
#define SCAONED_H

#include <dtOOTypeDef.h>

#include "scaFunction.h"
#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <string>

namespace dtOO {
class dtTransformer;
//! Function with scalar argument and scalar return value.
/*!
 * An instance of `scaOneD` is defined in the range [ _xxMin, _xxMax ]. This
 * class implements functions for handling scalar arguments.
 */
class scaOneD : public scaFunction {
public:
  dt__class(scaOneD, analyticFunction);
  scaOneD();
  scaOneD(scaOneD const &orig);
  virtual ~scaOneD();
  virtual scaOneD *clone(void) const = 0;
  virtual scaOneD *cloneTransformed(dtTransformer const *const dtT) const = 0;
  virtual scaOneD *create(void) const = 0;
  //! Function value.
  /*!
   * @param xx Argument.
   * @exception dtOO::eGeneral If dimension of xx>1.
   *
   */
  virtual dtReal YFloat(aFX const &xx) const;
  virtual dtReal YFloat(dtReal const &xx) const = 0;
  //! Inverse function.
  /*!
   * @param yy Function value.
   *
   */
  virtual dtReal invYFloat(dtReal const &yy) const;
  //! Function value at argument given in percent.
  /*!
   * @param xP Argument given in percent.
   *
   */
  dtReal YFloatPercent(dtReal const &xP) const;
  //! Define argument range of function.
  /*!
   * @param min Minimum argument value.
   * @param max Maximum argument value.
   *
   */
  void setMinMax(dtReal const min, dtReal const max);
  //! Define minimum argument bound of function.
  /*!
   * @param min Minimum argument value.
   *
   */
  void setMin(dtReal const min);
  //! Define maximum argument bound of function.
  /*!
   * @param max Maximum argument value.
   *
   */
  void setMax(dtReal const max);
  //! Return dimension of argument.
  virtual dtInt xDim(void) const;
  //! Return minimal argument bound of function.
  virtual dtReal xMin(dtInt const &dir) const;
  //! Return maximal argument bound of function.
  virtual dtReal xMax(dtInt const &dir) const;
  //! Compute length of function.
  /*!
   * Method approximates the length of the function f(x). This corresponds to:
   * @f[
   *   \int_{xMin}^{xMax} f dx
   * @f]
   * Integration is performed as numerical integration with a step size of
   * +1.0E-08.
   */
  dtReal getLength(void) const;
  //! Convert function argument from percent to current range.
  /*!
   * @param xx Percent value of function argument range.
   */
  dtReal x_percent(dtReal const &xx) const;
  //! Convert function argument from current range to percent.
  /*!
   * @param xx Current value of function argument range.
   */
  dtReal percent_x(dtReal const &xx) const;
  //! First derivative of function.
  /*!
   * @param xx Function argument value.
   *
   * Derivative is calculated numerically by an approximation.
   */
  dtReal DYFloat(dtReal const &xx) const;
  virtual vectorHandling<renderInterface *> getRender(void) const;

private:
  dtReal _xxMin;
  dtReal _xxMax;
};
dt__H_addCloneForpVH(scaOneD);
} // namespace dtOO
#endif /* SCAONED_H */
