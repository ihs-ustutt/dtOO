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

#ifndef feOpenFOAMTurboRule_H
#define feOpenFOAMTurboRule_H

#include <dtOOTypeDef.h>

#include "feOpenFOAMSetupRule.h"
#include <logMe/dtMacros.h>

namespace dtOO {
class feOpenFOAMTurboRule : public feOpenFOAMSetupRule {
public:
  dt__class(feOpenFOAMTurboRule, feOpenFOAMSetupRule);
  dt__classSelfCreate(feOpenFOAMTurboRule);
  feOpenFOAMTurboRule();
  virtual ~feOpenFOAMTurboRule();
  virtual std::vector<std::string> factoryAlias(void) const;
  virtual void executeOnVolVectorField(
    std::vector<std::string> const &rule, ::Foam::volVectorField &field
  ) const;
  virtual void executeOnVolScalarField(
    std::vector<std::string> const &rule, ::Foam::volScalarField &field
  ) const;

private:
  static bool _registrated;
};
} // namespace dtOO
#endif /* feOpenFOAMTurboRule_H */
