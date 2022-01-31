#ifndef feOpenFOAMEmptyRule_H
#define	feOpenFOAMEmptyRule_H

#include <dtOOTypeDef.h>

#include <logMe/dtMacros.h>
#include "feOpenFOAMSetupRule.h"

namespace dtOO {
  class feOpenFOAMEmptyRule : public feOpenFOAMSetupRule {
    public:
      dt__class(feOpenFOAMEmptyRule, feOpenFOAMSetupRule);  
      dt__classSelfCreate(feOpenFOAMEmptyRule);
      feOpenFOAMEmptyRule();
      virtual ~feOpenFOAMEmptyRule();
      virtual std::vector< std::string > factoryAlias( void ) const;
      virtual void executeOnMesh(
        std::vector< std::string > const & rule, ::Foam::polyMesh & mesh
      ) const;
    private:
      static bool _registrated;

  };
}
#endif	/* feOpenFOAMEmptyRule_H */

