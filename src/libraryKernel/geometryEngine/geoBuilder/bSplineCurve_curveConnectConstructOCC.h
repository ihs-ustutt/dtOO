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

#ifndef BSPLINECURVE_CURVECONNECTCONSTRUCTOCC_H
#define BSPLINECURVE_CURVECONNECTCONSTRUCTOCC_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <interfaceHeaven/ptrHandling.h>
#include <interfaceHeaven/vectorHandling.h>
#include <logMe/dtMacros.h>

#include <Precision.hxx>

namespace dtOO {
class dtCurve;

class bSplineCurve_curveConnectConstructOCC {
public:
  dt__classOnlyName(bSplineCurve_curveConnectConstructOCC);
  bSplineCurve_curveConnectConstructOCC(
    dtCurve const *const c0, dtCurve const *const c1
  );
  bSplineCurve_curveConnectConstructOCC(
    vectorHandling<dtCurve const *> const &cc,
    dtReal const &tol = Precision::Confusion(),
    dtInt const &param = 0,
    dtInt const &minM = 0
  );
  virtual ~bSplineCurve_curveConnectConstructOCC();
  dtCurve *result(void);

private:
  ptrHandling<dtCurve> _dtC;
};
} // namespace dtOO
#endif /* BSPLINECURVE_CURVECONNECTCONSTRUCTOCC_H */
