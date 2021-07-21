#ifndef feOpenFOAMGgiRule_H
#define	feOpenFOAMGgiRule_H

#include <logMe/dtMacros.h>
#include "feOpenFOAMSetupRule.h"

namespace dtOO {
  class feOpenFOAMGgiRule : public feOpenFOAMSetupRule {
    public:
      dt__class(feOpenFOAMGgiRule, feOpenFOAMSetupRule);  
      dt__classSelfCreate(feOpenFOAMGgiRule);
      feOpenFOAMGgiRule();
      virtual ~feOpenFOAMGgiRule();
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
#endif	/* feOpenFOAMGgiRule_H */

