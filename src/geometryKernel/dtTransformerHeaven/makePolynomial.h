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

#ifndef MAKEPOLYNOMIAL_H
#define MAKEPOLYNOMIAL_H

#include <dtOOTypeDef.h>

#include "dtTransformer.h"
#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>

namespace dtOO {
class analyticGeometry;

class makePolynomial : public dtTransformer {
public:
  dt__classOnlyName(makePolynomial);
  makePolynomial();
  makePolynomial(const makePolynomial &orig);
  virtual ~makePolynomial();
  virtual dtTransformer *clone(void) const;
  virtual dtTransformer *create(void) const;
  virtual bool isNecessary(void) const;
  virtual void init(
    ::QDomElement const *tE,
    baseContainer *const bC,
    lvH_constValue const *const cV,
    lvH_analyticFunction const *const aF,
    lvH_analyticGeometry const *const aG
  );
  virtual lvH_analyticGeometry apply(lvH_analyticGeometry const *const aGeoVecP
  ) const;

private:
  dtInt _nPointsOne;
  dtInt _nPointsTwo;
  dtInt _orderOne;
  dtInt _orderTwo;
  static bool _registrated;
};
} // namespace dtOO
#endif /* MAKEPOLYNOMIAL_H */
