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

#ifndef feOpenFOAMWallRule_H
#define	feOpenFOAMWallRule_H

#include <dtOOTypeDef.h>

#include <logMe/dtMacros.h>
#include "feOpenFOAMSetupRule.h"

namespace dtOO {
  class feOpenFOAMWallRule : public feOpenFOAMSetupRule {
    public:
      dt__class(feOpenFOAMWallRule, feOpenFOAMSetupRule);  
      dt__classSelfCreate(feOpenFOAMWallRule);
      feOpenFOAMWallRule();
      virtual ~feOpenFOAMWallRule();
      virtual std::vector< std::string > factoryAlias( void ) const;
      virtual void executeOnMesh(
        std::vector< std::string > const & rule, ::Foam::polyMesh & mesh
      ) const;       
    private:
      static bool _registrated;
  };
}
#endif	/* feOpenFOAMWallRule_H */
