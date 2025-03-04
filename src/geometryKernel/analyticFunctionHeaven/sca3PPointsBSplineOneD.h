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

#ifndef sca3PPointsBSplineOneD_H
#define sca3PPointsBSplineOneD_H

#include <dtOOTypeDef.h>

#include "analyticFunctionCompound.h"
#include "scaCurve2dOneD.h"
#include "scaOneD.h"
#include "scaOneDPolyInterface.h"
#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <vector>

namespace dtOO {
class dtTransformer;

class sca3PPointsBSplineOneD : public scaOneDPolyInterface {
public:
  dt__class(sca3PPointsBSplineOneD, analyticFunction);
  sca3PPointsBSplineOneD();
  sca3PPointsBSplineOneD(sca3PPointsBSplineOneD const &orig);
  sca3PPointsBSplineOneD(
    dtReal const &xB, dtReal const &yB, dtReal const &xA, dtReal const &yA
  );
  sca3PPointsBSplineOneD *clone(void) const;
  sca3PPointsBSplineOneD *cloneTransformed(dtTransformer const *const dtT
  ) const;
  sca3PPointsBSplineOneD *create(void) const;
  virtual ~sca3PPointsBSplineOneD();
  virtual dtReal YFloat(dtReal const &xx) const;
  virtual dtInt nDOF(void) const;
  virtual void setDOF(std::vector<dtReal> const value);

private:
  scaCurve2dOneD _curve;
};
typedef analyticFunctionCompound<sca3PPointsBSplineOneD>
  sca3PPointsBSplineOneDCompound;
} // namespace dtOO
#endif /* sca3PPointsBSplineOneD_H */
