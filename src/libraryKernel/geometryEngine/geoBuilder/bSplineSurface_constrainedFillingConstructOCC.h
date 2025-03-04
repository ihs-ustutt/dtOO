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

#ifndef bSplineSurface_constrainedFillingConstructOCC_H
#define bSplineSurface_constrainedFillingConstructOCC_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <interfaceHeaven/vectorHandling.h>
#include <logMe/dtMacros.h>

namespace dtOO {
class dtCurve;
class dtSurface;

class bSplineSurface_constrainedFillingConstructOCC {
public:
  dt__classOnlyName(bSplineSurface_constrainedFillingConstructOCC);

  bSplineSurface_constrainedFillingConstructOCC(
    dtCurve const *c0,
    dtCurve const *c1,
    dtCurve const *c2,
    dtCurve const *c3,
    dtInt const &maxDeg,
    dtInt const &maxSeg
  );
  virtual ~bSplineSurface_constrainedFillingConstructOCC();
  dtSurface *result(void);

private:
  ptrHandling<dtSurface> _dtS;
};
} // namespace dtOO
#endif /* bSplineSurface_constrainedFillingConstructOCC_H */
