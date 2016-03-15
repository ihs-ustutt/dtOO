#ifndef OPENFOAMWALLRULE_H
#define	OPENFOAMWALLRULE_H

#include <logMe/dtMacros.h>
#include "OpenFOAMSetupRule.h"

namespace dtOO {
  class OpenFOAMWallRule : public OpenFOAMSetupRule {
  public:
    dt__class(OpenFOAMWallRule, OpenFOAMSetupRule);  
    OpenFOAMWallRule();
    virtual ~OpenFOAMWallRule();
    virtual void executeOnMesh(
      std::vector< std::string > const & rule, ::Foam::polyMesh & mesh
    ) const;       
  private:

  };
}
#endif	/* OPENFOAMWALLRULE_H */

