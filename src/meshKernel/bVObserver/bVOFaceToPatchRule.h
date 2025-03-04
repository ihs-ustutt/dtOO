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

#ifndef bVOFaceToPatchRule_H
#define bVOFaceToPatchRule_H

#include <dtOOTypeDef.h>

#include "bVOInterface.h"
#include <logMe/dtMacros.h>

namespace dtOO {
class constValue;
class analyticFunction;
class analyticGeometry;

class bVOFaceToPatchRule : public bVOInterface {
public:
  dt__class(bVOFaceToPatchRule, bVOInterface);
  dt__classSelfCreate(bVOFaceToPatchRule);
  bVOFaceToPatchRule();
  virtual ~bVOFaceToPatchRule();
  virtual std::vector<std::string> factoryAlias(void) const;
  virtual void init(
    ::QDomElement const &element,
    baseContainer const *const bC,
    lvH_constValue const *const cV,
    lvH_analyticFunction const *const aF,
    lvH_analyticGeometry const *const aG,
    lvH_boundedVolume const *const bV,
    boundedVolume *attachTo
  );
  virtual void postUpdate(void);

private:
  static bool _registrated;
};
} // namespace dtOO
#endif /* bVOFaceToPatchRule_H */
