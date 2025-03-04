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

#ifndef map3dTo3dWithInternalBlockGmsh_H
#define map3dTo3dWithInternalBlockGmsh_H

#include <dtOOTypeDef.h>

#include "gmshBoundedVolume.h"
#include <logMe/dtMacros.h>

namespace dtOO {
class baseContainer;
class constValue;
class analyticFunction;
class analyticGeometry;
class map3dTo3d;
class map2dTo3d;

class map3dTo3dWithInternalBlockGmsh : public gmshBoundedVolume {
public:
  dt__class(map3dTo3dWithInternalBlockGmsh, boundedVolume);
  dt__classSelfCreate(map3dTo3dWithInternalBlockGmsh);
  map3dTo3dWithInternalBlockGmsh();
  virtual ~map3dTo3dWithInternalBlockGmsh();
  virtual void init(
    ::QDomElement const &element,
    baseContainer *const bC,
    lvH_constValue const *const cV,
    lvH_analyticFunction const *const aF,
    lvH_analyticGeometry const *const aG,
    lvH_boundedVolume const *const bV
  );

private:
  dt__pH(map3dTo3d) _m3d;
  dt__pVH(map3dTo3d) _internal;
  static bool _registrated;
};
} // namespace dtOO
#endif /* map3dTo3dWithInternalBlockGmsh_H */
