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

#ifndef scaTanhUnitGradingOneD_H
#define scaTanhUnitGradingOneD_H

#include <dtOOTypeDef.h>

#include "analyticFunctionCompound.h"
#include "scaOneD.h"
#include "scaOneDPolyInterface.h"
#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <vector>

namespace dtOO {
class dtTransformer;

//! Unit-interval scalar hyperbolic-tangent grading with adjustable
//! coefficients.
class scaTanhUnitGradingOneD : public scaOneDPolyInterface {
public:
  dt__class(scaTanhUnitGradingOneD, analyticFunction);
  scaTanhUnitGradingOneD();
  scaTanhUnitGradingOneD(scaTanhUnitGradingOneD const &orig);
  /*!
   * @param cc Coefficients of the unit-interval grading expression.
   * @param gg Current grading factor.
   * @param ggMin Minimum permitted grading factor.
   * @param ggMax Maximum permitted grading factor.
   */
  scaTanhUnitGradingOneD(
    std::vector<dtReal> cc,
    dtReal const &gg,
    dtReal const &ggMin,
    dtReal const &ggMax
  );
  scaTanhUnitGradingOneD *clone(void) const;
  scaTanhUnitGradingOneD *cloneTransformed(dtTransformer const *const dtT
  ) const;
  scaTanhUnitGradingOneD *create(void) const;
  virtual ~scaTanhUnitGradingOneD();
  virtual dtReal YFloat(dtReal const &xx) const;
  //! Return the number of grading degrees of freedom.
  virtual dtInt nDOF(void) const;
  //! Update the grading degrees of freedom.
  virtual void setDOF(std::vector<dtReal> const value);

private:
  dtReal _gg;
  dtReal _ggMin;
  dtReal _ggMax;
  std::vector<dtReal> _cc;
};
//! Compound collection of unit-interval hyperbolic-tangent grading functions.
typedef analyticFunctionCompound<scaTanhUnitGradingOneD>
  scaTanhUnitGradingOneDCompound;
} // namespace dtOO
#endif /* scaTanhUnitGradingOneD_H */
