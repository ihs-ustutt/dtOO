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

#ifndef MAP2DTO3DTRIANGULATED_H
#define MAP2DTO3DTRIANGULATED_H

#include <dtOOTypeDef.h>

#include "gmshBoundedVolume.h"
#include <logMe/dtMacros.h>

namespace dtOO {
class baseContainer;
class constValue;
class analyticFunction;
class analyticGeometry;
class map2dTo3d;

class map2dTo3dTriangulated : public gmshBoundedVolume {
public:
  dt__class(map2dTo3dTriangulated, boundedVolume);
  dt__classSelfCreate(map2dTo3dTriangulated);
  map2dTo3dTriangulated();
  virtual ~map2dTo3dTriangulated();
  virtual void init(
    ::QDomElement const &element,
    baseContainer *const bC,
    lvH_constValue const *const cV,
    lvH_analyticFunction const *const aF,
    lvH_analyticGeometry const *const aG,
    lvH_boundedVolume const *const bV
  );

private:
  static bool _registrated;
};
} // namespace dtOO

#endif /* MAP2DTO3DTRIANGULATED_H */
