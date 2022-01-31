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

