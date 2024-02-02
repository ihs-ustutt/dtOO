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

