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

#ifndef bSplineCurve_approxGeomCurve2dInGeomSurface_H
#define bSplineCurve_approxGeomCurve2dInGeomSurface_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <interfaceHeaven/ptrHandling.h>
#include <logMe/dtMacros.h>

namespace dtOO {
class dtCurve;
class dtCurve2d;
class dtSurface;

class bSplineCurve_approxGeomCurve2dInGeomSurface {
public:
  dt__classOnlyName(bSplineCurve_approxGeomCurve2dInGeomSurface);
  bSplineCurve_approxGeomCurve2dInGeomSurface(
    dtCurve2d const *const dtC2d, dtSurface const *const dtS
  );
  virtual ~bSplineCurve_approxGeomCurve2dInGeomSurface();
  dtCurve *result(void);

private:
  dt__pH(dtCurve) _dtC;
};
} // namespace dtOO
#endif /* bSplineCurve_approxGeomCurve2dInGeomSurface_H */
