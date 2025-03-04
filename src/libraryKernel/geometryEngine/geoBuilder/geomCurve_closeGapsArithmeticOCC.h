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

#ifndef geomCurve_closeGapsArithmeticOCC_H
#define geomCurve_closeGapsArithmeticOCC_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <interfaceHeaven/ptrHandling.h>
#include <interfaceHeaven/vectorHandling.h>
#include <logMe/dtMacros.h>

namespace dtOO {
class dtCurve;

class geomCurve_closeGapsArithmeticOCC {
public:
  dt__classOnlyName(geomCurve_closeGapsArithmeticOCC);
  geomCurve_closeGapsArithmeticOCC();
  geomCurve_closeGapsArithmeticOCC(dt__pVH(dtCurve) const &dtC);
  virtual ~geomCurve_closeGapsArithmeticOCC();
  dt__pVH(dtCurve) result(void);

private:
  dt__pVH(dtCurve) _dtC;
};
} // namespace dtOO
#endif /* geomCurve_closeGapsArithmeticOCC_H */
