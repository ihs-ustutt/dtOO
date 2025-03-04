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

#ifndef bSplineCurve2d_angleRatioDeltaYPointOnePointTwoConstructOCC_H
#define bSplineCurve2d_angleRatioDeltaYPointOnePointTwoConstructOCC_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <interfaceHeaven/ptrHandling.h>
#include <logMe/dtMacros.h>

namespace dtOO {
class dtCurve2d;

class bSplineCurve2d_angleRatioDeltaYPointOnePointTwoConstructOCC {
public:
  dt__classOnlyName(bSplineCurve2d_angleRatioDeltaYPointOnePointTwoConstructOCC
  );
  bSplineCurve2d_angleRatioDeltaYPointOnePointTwoConstructOCC(
    dtReal angleIn,
    dtReal angleOut,
    dtReal const &ratio,
    dtReal const &deltaY,
    dtReal const &pointOne,
    dtReal const &pointTwo
  );
  virtual ~bSplineCurve2d_angleRatioDeltaYPointOnePointTwoConstructOCC();
  dtCurve2d *result(void);

private:
  ptrHandling<dtCurve2d> _dtC2d;
};
} // namespace dtOO
#endif /* bSplineCurve2d_angleRatioDeltaYPointOnePointTwoConstructOCC_H */
