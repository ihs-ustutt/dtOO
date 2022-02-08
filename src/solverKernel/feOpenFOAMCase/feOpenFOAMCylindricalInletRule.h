#ifndef feOpenFOAMCylindricalInletRule_H
#define	feOpenFOAMCylindricalInletRule_H

#include <dtOOTypeDef.h>

#include <logMe/dtMacros.h>
#include "feOpenFOAMSetupRule.h"

namespace dtOO {
  class feOpenFOAMCylindricalInletRule : public feOpenFOAMSetupRule {
    public:
      dt__class(feOpenFOAMCylindricalInletRule, feOpenFOAMSetupRule); 
      dt__classSelfCreate(feOpenFOAMCylindricalInletRule);
      feOpenFOAMCylindricalInletRule();
      virtual ~feOpenFOAMCylindricalInletRule();
      virtual std::vector< std::string > factoryAlias( void ) const;
      void executeOnVolVectorField(
        std::vector< std::string > const & rule, ::Foam::volVectorField & field
      ) const;
    private:
      static bool _registrated;
  };
}
#endif	/* feOpenFOAMCylindricalInletRule_H */

