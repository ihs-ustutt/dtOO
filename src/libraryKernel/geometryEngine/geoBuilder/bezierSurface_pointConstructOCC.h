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

#ifndef BEZIERSURFACEPOINTCONSTRUCTOCC_H
#define BEZIERSURFACEPOINTCONSTRUCTOCC_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <interfaceHeaven/ptrHandling.h>
#include <interfaceHeaven/twoDArrayHandling.h>
#include <logMe/dtMacros.h>

namespace dtOO {
class dtSurface;

class bezierSurface_pointConstructOCC {
public:
  dt__classOnlyName(bezierSurface_pointConstructOCC);
  bezierSurface_pointConstructOCC(twoDArrayHandling<dtPoint3> const &pp);
  virtual ~bezierSurface_pointConstructOCC();
  dtSurface *result(void);

private:
  ptrHandling<dtSurface> _dtS;
};
} // namespace dtOO
#endif /* BEZIERSURFACEPOINTCONSTRUCTOCC_H */
