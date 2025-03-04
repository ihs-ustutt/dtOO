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

#ifndef geomSurface_baseConstructOCC_H
#define geomSurface_baseConstructOCC_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <interfaceHeaven/ptrHandling.h>
#include <interfaceHeaven/vectorHandling.h>
#include <logMe/dtMacros.h>

namespace dtOO {
class dtSurface;
class dtOCCSurfaceBase;

class geomSurface_baseConstructOCC {
public:
  dt__classOnlyName(geomSurface_baseConstructOCC);
  geomSurface_baseConstructOCC(dtOCCSurfaceBase const &base);
  virtual ~geomSurface_baseConstructOCC();
  static bool support(dtOCCSurfaceBase const &base);
  dtSurface *result(void);

private:
  ptrHandling<dtSurface> _dtS;
};
} // namespace dtOO
#endif /* geomSurface_baseConstructOCC_H */
