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

#ifndef BSPLINESURFACE_SKINCONSTRUCTOCC_H
#define BSPLINESURFACE_SKINCONSTRUCTOCC_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <interfaceHeaven/ptrHandling.h>
#include <interfaceHeaven/vectorHandling.h>
#include <logMe/dtMacros.h>

namespace dtOO {
class dtSurface;
class dtCurve;

class bSplineSurface_skinConstructOCC {
public:
  dt__classOnlyName(bSplineSurface_skinConstructOCC);
  bSplineSurface_skinConstructOCC();
  bSplineSurface_skinConstructOCC(
    dtCurve const *const c0, dtCurve const *const c1
  );
  bSplineSurface_skinConstructOCC(vectorHandling<dtCurve const *> const &cL);
  bSplineSurface_skinConstructOCC(
    vectorHandling<dtCurve const *> const &cL,
    dtInt const &minDeg,
    dtInt const &maxDeg,
    dtInt const &nIter
  );
  virtual ~bSplineSurface_skinConstructOCC();
  dtSurface *result(void);

private:
  ptrHandling<dtSurface> _dtS;
};
} // namespace dtOO
#endif /* BSPLINESURFACE_SKINCONSTRUCTOCC_H */
