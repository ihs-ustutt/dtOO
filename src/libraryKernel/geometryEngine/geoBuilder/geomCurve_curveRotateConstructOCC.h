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

#ifndef geomCurve_curveRotateConstructOCC_H
#define geomCurve_curveRotateConstructOCC_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <interfaceHeaven/ptrHandling.h>
#include <interfaceHeaven/vectorHandling.h>
#include <logMe/dtMacros.h>

namespace dtOO {
class dtCurve;

class geomCurve_curveRotateConstructOCC {
public:
  dt__classOnlyName(geomCurve_curveRotateConstructOCC);
  geomCurve_curveRotateConstructOCC();
  geomCurve_curveRotateConstructOCC(
    dtCurve const *const dtC,
    dtPoint3 const &oP,
    dtVector3 const &rA,
    dtReal const &angle
  );
  virtual ~geomCurve_curveRotateConstructOCC();
  dtCurve *result(void);

private:
  ptrHandling<dtCurve> _dtC;
};
} // namespace dtOO
#endif /* geomCurve_curveRotateConstructOCC_H */
