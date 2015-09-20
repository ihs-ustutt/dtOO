#ifndef openFOAMCellZoneRule_H
#define	openFOAMCellZoneRule_H

#include <logMe/dtMacros.h>
#include "openFOAMSetupRule.h"

namespace dtOO {
  class openFOAMCellZoneRule : public openFOAMSetupRule {
  public:
    dt__class(openFOAMCellZoneRule, openFOAMSetupRule);  
    openFOAMCellZoneRule();
    virtual ~openFOAMCellZoneRule();
    virtual void executeOnMesh(
      std::vector< std::string > const & rule, ::Foam::polyMesh & mesh
    ) const;
  private:

  };
}
#endif	/* openFOAMCellZoneRule_H */

