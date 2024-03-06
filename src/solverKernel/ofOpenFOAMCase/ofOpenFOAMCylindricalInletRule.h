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

#ifndef ofOpenFOAMCylindricalInletRule_H
#define	ofOpenFOAMCylindricalInletRule_H

#include <dtOOTypeDef.h>

#include <logMe/dtMacros.h>
#include "ofOpenFOAMSetupRule.h"

namespace dtOO {
  class ofOpenFOAMCylindricalInletRule : public ofOpenFOAMSetupRule {
    public:
      dt__class(ofOpenFOAMCylindricalInletRule, ofOpenFOAMSetupRule); 
      dt__classSelfCreate(ofOpenFOAMCylindricalInletRule);
      ofOpenFOAMCylindricalInletRule();
      virtual ~ofOpenFOAMCylindricalInletRule();
      virtual std::vector< std::string > factoryAlias( void ) const;
      void executeOnVolVectorField(
        std::vector< std::string > const & rule, ::Foam::volVectorField & field
      ) const;
    private:
      static bool _registrated;
  };
}
#endif	/* ofOpenFOAMCylindricalInletRule_H */
