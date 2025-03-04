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

#ifndef ofOpenFOAMWallRule_H
#define ofOpenFOAMWallRule_H

#include <dtOOTypeDef.h>

#include "ofOpenFOAMSetupRule.h"
#include <logMe/dtMacros.h>

namespace dtOO {
class ofOpenFOAMWallRule : public ofOpenFOAMSetupRule {
public:
  dt__class(ofOpenFOAMWallRule, ofOpenFOAMSetupRule);
  dt__classSelfCreate(ofOpenFOAMWallRule);
  ofOpenFOAMWallRule();
  virtual ~ofOpenFOAMWallRule();
  virtual std::vector<std::string> factoryAlias(void) const;
  virtual void executeOnMesh(
    std::vector<std::string> const &rule, ::Foam::polyMesh &mesh
  ) const;

private:
  static bool _registrated;
};
} // namespace dtOO
#endif /* ofOpenFOAMWallRule_H */
