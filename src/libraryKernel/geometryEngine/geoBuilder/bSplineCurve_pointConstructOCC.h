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

#ifndef bSplineCurve_pointConstructOCC_H
#define bSplineCurve_pointConstructOCC_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <interfaceHeaven/ptrHandling.h>
#include <logMe/dtMacros.h>

namespace dtOO {
class dtCurve;

class bSplineCurve_pointConstructOCC {
public:
  dt__classOnlyName(bSplineCurve_pointConstructOCC);
  bSplineCurve_pointConstructOCC(
    std::vector<dtPoint3> const &pp, dtInt const &order
  );
  bSplineCurve_pointConstructOCC(dtPoint3 const &p0, dtPoint3 const &p1);
  virtual ~bSplineCurve_pointConstructOCC();
  dtCurve *result(void);

private:
  dt__pH(dtCurve) _dtC;
};
} // namespace dtOO
#endif /* bSplineCurve_pointConstructOCC_H */
