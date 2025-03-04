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

#ifndef BEZIERCURVEPOINTCONSTRUCTOCC_H
#define BEZIERCURVEPOINTCONSTRUCTOCC_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <interfaceHeaven/ptrHandling.h>
#include <logMe/dtMacros.h>

namespace dtOO {
class dtCurve;

class bezierCurve_pointConstructOCC {
public:
  dt__classOnlyName(bezierCurve_pointConstructOCC);
  bezierCurve_pointConstructOCC(std::vector<dtPoint3> const &pp);
  virtual ~bezierCurve_pointConstructOCC();
  dtCurve *result(void);

private:
  ptrHandling<dtCurve> _dtC;
};
} // namespace dtOO
#endif /* BEZIERCURVEPOINTCONSTRUCTOCC_H */
