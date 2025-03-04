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

#ifndef geomShape_readOCC_H
#define geomShape_readOCC_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <interfaceHeaven/ptrHandling.h>
#include <interfaceHeaven/vectorHandling.h>
#include <logMe/dtMacros.h>

namespace dtOO {
class dtCurve;
class dtSurface;

class geomShape_readOCC {
public:
  dt__classOnlyName(geomShape_readOCC);
  geomShape_readOCC();
  geomShape_readOCC(std::string const &filename);
  virtual ~geomShape_readOCC();
  vectorHandling<dtCurve *> resultCurveVectorHandling(void) const;
  vectorHandling<dtSurface *> resultSurfaceVectorHandling(void) const;
  ptrVectorHandling<dtCurve> const &resultCurveRef(void) const;
  ptrVectorHandling<dtSurface> const &resultSurfaceRef(void) const;

private:
  ptrVectorHandling<dtCurve> _dtC;
  ptrVectorHandling<dtSurface> _dtS;
};
} // namespace dtOO
#endif /* geomShape_readOCC_H */
