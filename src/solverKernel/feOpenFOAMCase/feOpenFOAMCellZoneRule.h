#ifndef feOpenFOAMCellZoneRule_H
#define	feOpenFOAMCellZoneRule_H

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

