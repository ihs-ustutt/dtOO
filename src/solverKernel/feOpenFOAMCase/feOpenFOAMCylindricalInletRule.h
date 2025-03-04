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

#ifndef feOpenFOAMCylindricalInletRule_H
#define feOpenFOAMCylindricalInletRule_H

#include <dtOOTypeDef.h>

#include "feOpenFOAMSetupRule.h"
#include <logMe/dtMacros.h>

namespace dtOO {
class feOpenFOAMCylindricalInletRule : public feOpenFOAMSetupRule {
public:
  dt__class(feOpenFOAMCylindricalInletRule, feOpenFOAMSetupRule);
  dt__classSelfCreate(feOpenFOAMCylindricalInletRule);
  feOpenFOAMCylindricalInletRule();
  virtual ~feOpenFOAMCylindricalInletRule();
  virtual std::vector<std::string> factoryAlias(void) const;
  void executeOnVolVectorField(
    std::vector<std::string> const &rule, ::Foam::volVectorField &field
  ) const;

private:
  static bool _registrated;
};
} // namespace dtOO
#endif /* feOpenFOAMCylindricalInletRule_H */
