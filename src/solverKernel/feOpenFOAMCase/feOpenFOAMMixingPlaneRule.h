#ifndef feOpenFOAMMixingPlaneRule_H
#define	feOpenFOAMMixingPlaneRule_H

#include <dtOOTypeDef.h>

#include <logMe/dtMacros.h>
#include "feOpenFOAMSetupRule.h"

namespace dtOO {
  class feOpenFOAMMixingPlaneRule : public feOpenFOAMSetupRule {
    public:
      dt__class(feOpenFOAMMixingPlaneRule, feOpenFOAMSetupRule);  
      dt__classSelfCreate(feOpenFOAMMixingPlaneRule);
      feOpenFOAMMixingPlaneRule();
      virtual ~feOpenFOAMMixingPlaneRule();
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
#endif	/* feOpenFOAMMixingPlaneRule_H */

