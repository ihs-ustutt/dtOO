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

#ifndef geomSurface_surfaceReverseConstructOCC_H
#define geomSurface_surfaceReverseConstructOCC_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <interfaceHeaven/ptrHandling.h>
#include <interfaceHeaven/vectorHandling.h>
#include <logMe/dtMacros.h>

namespace dtOO {
class dtSurface;

class geomSurface_surfaceReverseConstructOCC {
public:
  dt__classOnlyName(geomSurface_surfaceReverseConstructOCC);
  geomSurface_surfaceReverseConstructOCC();
  geomSurface_surfaceReverseConstructOCC(
    dtSurface const *const dtS, bool const &revU, bool const &revV
  );
  virtual ~geomSurface_surfaceReverseConstructOCC();
  dtSurface *result(void);

private:
  ptrHandling<dtSurface> _dtS;
};
} // namespace dtOO
#endif /* geomSurface_surfaceReverseConstructOCC_H */
