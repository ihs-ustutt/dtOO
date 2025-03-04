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

#ifndef bVORemoveRegions_H
#define bVORemoveRegions_H

#include <dtOOTypeDef.h>

#include "bVOInterface.h"
#include <dtLinearAlgebra.h>
#include <interfaceHeaven/twoDArrayHandling.h>
#include <logMe/dtMacros.h>

namespace dtOO {
class boundedVolume;
class dtGmshEdge;

class bVORemoveRegions : public bVOInterface {
public:
  dt__class(bVORemoveRegions, bVOInterface);
  dt__classSelfCreate(bVORemoveRegions);
  bVORemoveRegions();
  virtual ~bVORemoveRegions();
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
  using bVOInterface::postUpdate;

private:
  std::vector<std::string> _regionLabel;
  static bool _registrated;
};
} // namespace dtOO
#endif /* bVORemoveRegions_H */
