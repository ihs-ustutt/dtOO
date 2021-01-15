#ifndef OpenFOAMCellZoneRule_H
#define	OpenFOAMCellZoneRule_H

#include <logMe/dtMacros.h>
#include "OpenFOAMSetupRule.h"

namespace dtOO {
  class OpenFOAMCellZoneRule : public OpenFOAMSetupRule {
    public:
      dt__class(OpenFOAMCellZoneRule, OpenFOAMSetupRule);  
      OpenFOAMCellZoneRule();
      virtual ~OpenFOAMCellZoneRule();
      virtual void executeOnMesh(
        std::vector< std::string > const & rule, ::Foam::polyMesh & mesh
      ) const;
    private:

  };
}
#endif	/* OpenFOAMCellZoneRule_H */

