#ifndef feOpenFOAMCyclicGgiRule_H
#define	feOpenFOAMCyclicGgiRule_H

#include <logMe/dtMacros.h>
#include "feOpenFOAMSetupRule.h"

namespace dtOO {
  class feOpenFOAMCyclicGgiRule : public feOpenFOAMSetupRule {
    public:
      dt__class(feOpenFOAMCyclicGgiRule, feOpenFOAMSetupRule);  
      dt__classSelfCreate(feOpenFOAMCyclicGgiRule);
      feOpenFOAMCyclicGgiRule();
      virtual ~feOpenFOAMCyclicGgiRule();
      virtual std::vector< std::string > factoryAlias( void ) const;
      virtual void executeOnMesh(
        std::vector< std::string > const & rule, ::Foam::polyMesh & mesh
      ) const;
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
#endif	/* feOpenFOAMCyclicGgiRule_H */

