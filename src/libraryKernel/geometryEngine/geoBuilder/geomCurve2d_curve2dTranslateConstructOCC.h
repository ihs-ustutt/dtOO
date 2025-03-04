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

#ifndef geomCurve2d_curve2dTranslateConstructOCC_H
#define geomCurve2d_curve2dTranslateConstructOCC_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <interfaceHeaven/ptrHandling.h>
#include <interfaceHeaven/vectorHandling.h>
#include <logMe/dtMacros.h>

namespace dtOO {
class dtCurve2d;

class geomCurve2d_curve2dTranslateConstructOCC {
public:
  dt__classOnlyName(geomCurve2d_curve2dTranslateConstructOCC);
  geomCurve2d_curve2dTranslateConstructOCC();
  geomCurve2d_curve2dTranslateConstructOCC(
    dtCurve2d const *const dtC2d, dtVector2 const &vv
  );
  virtual ~geomCurve2d_curve2dTranslateConstructOCC();
  dtCurve2d *result(void);

private:
  ptrHandling<dtCurve2d> _dtC2d;
};
} // namespace dtOO
#endif /* geomCurve2d_curve2dTranslateConstructOCC_H */
