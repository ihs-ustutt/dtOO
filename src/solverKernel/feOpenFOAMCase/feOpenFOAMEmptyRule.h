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

#ifndef feOpenFOAMEmptyRule_H
#define feOpenFOAMEmptyRule_H

#include <dtOOTypeDef.h>

#include "feOpenFOAMSetupRule.h"
#include <logMe/dtMacros.h>

namespace dtOO {
class feOpenFOAMEmptyRule : public feOpenFOAMSetupRule {
public:
  dt__class(feOpenFOAMEmptyRule, feOpenFOAMSetupRule);
  dt__classSelfCreate(feOpenFOAMEmptyRule);
  feOpenFOAMEmptyRule();
  virtual ~feOpenFOAMEmptyRule();
  virtual std::vector<std::string> factoryAlias(void) const;
  virtual void executeOnMesh(
    std::vector<std::string> const &rule, ::Foam::polyMesh &mesh
  ) const;

private:
  static bool _registrated;
};
} // namespace dtOO
#endif /* feOpenFOAMEmptyRule_H */
