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

#ifndef TRIMMEDCURVE2D_TWOPOINTSCONNECTCONSTRUCT_H
#define TRIMMEDCURVE2D_TWOPOINTSCONNECTCONSTRUCT_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <interfaceHeaven/ptrHandling.h>
#include <logMe/dtMacros.h>

namespace dtOO {
class dtCurve2d;

class trimmedCurve2d_twoPointsConnectConstructOCC {
public:
  dt__classOnlyName(trimmedCurve2d_twoPointsConnectConstructOCC);
  trimmedCurve2d_twoPointsConnectConstructOCC(
    dtPoint2 const &p0, dtPoint2 const &p1
  );
  virtual ~trimmedCurve2d_twoPointsConnectConstructOCC();
  dtCurve2d *result(void);

private:
  ptrHandling<dtCurve2d> _dtC2d;
};
} // namespace dtOO
#endif /* TRIMMEDCURVE2D_TWOPOINTSCONNECTCONSTRUCT_H */
