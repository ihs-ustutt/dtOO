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

#ifndef geomCurve_curveReverseConstructOCC_H
#define geomCurve_curveReverseConstructOCC_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <interfaceHeaven/ptrHandling.h>
#include <interfaceHeaven/vectorHandling.h>
#include <logMe/dtMacros.h>

namespace dtOO {
class dtCurve;

class geomCurve_curveReverseConstructOCC {
public:
  dt__classOnlyName(geomCurve_curveReverseConstructOCC);
  geomCurve_curveReverseConstructOCC();
  geomCurve_curveReverseConstructOCC(
    dtCurve const *const dtC, bool const &revU
  );
  virtual ~geomCurve_curveReverseConstructOCC();
  dtCurve *result(void);

private:
  ptrHandling<dtCurve> _dtC;
};
} // namespace dtOO
#endif /* geomCurve_curveReverseConstructOCC_H */
