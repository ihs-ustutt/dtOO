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

#ifndef bSplineCurve_normalOffsetGeomCurveOCC_H
#define bSplineCurve_normalOffsetGeomCurveOCC_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <interfaceHeaven/ptrHandling.h>
#include <logMe/dtMacros.h>

namespace dtOO {
class dtCurve;

class bSplineCurve_normalOffsetGeomCurveOCC {
public:
  dt__classOnlyName(bSplineCurve_normalOffsetGeomCurveOCC);
  bSplineCurve_normalOffsetGeomCurveOCC(
    dtCurve const *const dtC, dtReal const &offset, dtVector3 const &nn
  );
  virtual ~bSplineCurve_normalOffsetGeomCurveOCC();
  dtCurve *result(void);

private:
  ptrHandling<dtCurve> _dtC;
};
} // namespace dtOO

#endif /* bSplineCurve_normalOffsetGeomCurveOCC_H */
