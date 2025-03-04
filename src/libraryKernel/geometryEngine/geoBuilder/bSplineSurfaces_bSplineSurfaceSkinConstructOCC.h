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

#ifndef bSplineSurfaces_bSplineSurfaceSkinConstructOCC_H
#define bSplineSurfaces_bSplineSurfaceSkinConstructOCC_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <interfaceHeaven/vectorHandling.h>
#include <logMe/dtMacros.h>

namespace dtOO {
class dtSurface;

class bSplineSurfaces_bSplineSurfaceSkinConstructOCC {
public:
  dt__classOnlyName(bSplineSurfaces_bSplineSurfaceSkinConstructOCC);
  bSplineSurfaces_bSplineSurfaceSkinConstructOCC(dt__vH(dtSurface const *)
                                                   const &dtS);
  bSplineSurfaces_bSplineSurfaceSkinConstructOCC(
    dtSurface const *const dtS0, dtSurface const *const dtS1
  );
  virtual ~bSplineSurfaces_bSplineSurfaceSkinConstructOCC();
  vectorHandling<dtSurface *> result(void);

private:
  dt__vH(dtSurface *) _dtS;
};
} // namespace dtOO
#endif /* bSplineSurfaces_bSplineSurfaceSkinConstructOCC_H */
