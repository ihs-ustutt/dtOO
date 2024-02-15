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

#ifndef feOpenFOAMCellZoneRule_H
#define	feOpenFOAMCellZoneRule_H

#include <dtOOTypeDef.h>

#include <logMe/dtMacros.h>
#include "feOpenFOAMSetupRule.h"

namespace dtOO {
  class feOpenFOAMCellZoneRule : public feOpenFOAMSetupRule {
    public:
      dt__class(feOpenFOAMCellZoneRule, feOpenFOAMSetupRule);  
      dt__classSelfCreate(feOpenFOAMCellZoneRule);
      feOpenFOAMCellZoneRule();
      virtual ~feOpenFOAMCellZoneRule();
      virtual std::vector< std::string > factoryAlias( void ) const;
      virtual void executeOnMesh(
        std::vector< std::string > const & rule, ::Foam::polyMesh & mesh
      ) const;
    private:
      static bool _registrated;
  };
}
#endif	/* feOpenFOAMCellZoneRule_H */
