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

#ifndef TRIMMEDCURVE_TWOPOINTSCONNECTCONSTRUCT_H
#define TRIMMEDCURVE_TWOPOINTSCONNECTCONSTRUCT_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <interfaceHeaven/ptrHandling.h>
#include <logMe/dtMacros.h>

namespace dtOO {
class dtCurve;

class trimmedCurve_twoPointsConnectConstructOCC {
public:
  dt__classOnlyName(trimmedCurve_twoPointsConnectConstructOCC);
  trimmedCurve_twoPointsConnectConstructOCC(
    dtPoint3 const &p0, dtPoint3 const &p1
  );
  virtual ~trimmedCurve_twoPointsConnectConstructOCC();
  dtCurve *result(void);

private:
  ptrHandling<dtCurve> _dtC;
};
} // namespace dtOO
#endif /* TRIMMEDCURVE_TWOPOINTSCONNECTCONSTRUCT_H */
