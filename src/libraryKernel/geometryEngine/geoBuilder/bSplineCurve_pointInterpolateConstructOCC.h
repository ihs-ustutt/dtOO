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

#ifndef bSplineCurve_pointInterpolateConstructOCC_H
#define bSplineCurve_pointInterpolateConstructOCC_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>

namespace dtOO {
class dtCurve;

class bSplineCurve_pointInterpolateConstructOCC {
public:
  dt__classOnlyName(bSplineCurve_pointInterpolateConstructOCC);
  bSplineCurve_pointInterpolateConstructOCC(std::vector<dtPoint3> const &pp);
  bSplineCurve_pointInterpolateConstructOCC(
    std::vector<dtPoint3> const &pp, dtInt const &degMin, dtInt const &degMax
  );
  bSplineCurve_pointInterpolateConstructOCC(
    std::vector<dtPoint3> const &pp,
    dtInt const &degMin,
    dtInt const &degMax,
    dtInt const &cont
  );
  virtual ~bSplineCurve_pointInterpolateConstructOCC();
  dtCurve *result(void);

private:
  dt__pH(dtCurve) _dtC;
};
} // namespace dtOO
#endif /* bSplineCurve_pointInterpolateConstructOCC_H */
