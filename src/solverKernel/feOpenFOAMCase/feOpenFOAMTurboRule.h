#ifndef feOpenFOAMTurboRule_H
#define	feOpenFOAMTurboRule_H

#include <logMe/dtMacros.h>
#include "feOpenFOAMSetupRule.h"

namespace dtOO {
  class feOpenFOAMTurboRule : public feOpenFOAMSetupRule {
    public:
      dt__class(feOpenFOAMTurboRule, feOpenFOAMSetupRule);    
      dt__classSelfCreate(feOpenFOAMTurboRule);
      feOpenFOAMTurboRule();
      virtual ~feOpenFOAMTurboRule();
      virtual std::vector< std::string > factoryAlias( void ) const;
      virtual void executeOnVolVectorField(
        std::vector< std::string > const & rule, ::Foam::volVectorField & field
      ) const;    
      virtual void executeOnVolScalarField(
        std::vector< std::string > const & rule, ::Foam::volScalarField & field
      ) const;
    private:
      static bool _registrated;      
  };
}
#endif	/* feOpenFOAMTurboRule_H */

