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

#ifndef bVOAddInternalEdge_H
#define bVOAddInternalEdge_H

#include <dtOOTypeDef.h>

#include "bVOInterface.h"
#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <vector>

namespace dtOO {
class boundedVolume;
class dtGmshEdge;
class map1dTo3d;

class bVOAddInternalEdge : public bVOInterface {
public:
  dt__class(bVOAddInternalEdge, bVOInterface);
  dt__classSelfCreate(bVOAddInternalEdge);
  bVOAddInternalEdge();
  virtual ~bVOAddInternalEdge();
  virtual void init(
    ::QDomElement const &element,
    baseContainer const *const bC,
    lvH_constValue const *const cV,
    lvH_analyticFunction const *const aF,
    lvH_analyticGeometry const *const aG,
    lvH_boundedVolume const *const bV,
    boundedVolume *attachTo
  );
  virtual void preUpdate(void);

private:
  static bool _registrated;
};
} // namespace dtOO
#endif /* bVOAddInternalEdge_H */
