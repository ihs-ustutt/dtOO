#ifndef OPENFOAMWALLRULE_H
#define	OPENFOAMWALLRULE_H

#include <logMe/dtMacros.h>
#include "openFOAMSetupRule.h"

namespace dtOO {
  class openFOAMWallRule : public openFOAMSetupRule {
  public:
    dt__class(openFOAMWallRule, openFOAMSetupRule);  
    openFOAMWallRule();
    virtual ~openFOAMWallRule();
    virtual void executeOnMesh(
      std::vector< std::string > const & rule, ::Foam::polyMesh & mesh
    ) const;       
  private:

  };
}
#endif	/* OPENFOAMWALLRULE_H */

