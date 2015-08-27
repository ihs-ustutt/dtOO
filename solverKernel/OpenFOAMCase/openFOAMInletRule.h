#ifndef openFOAMInletRule_H
#define	openFOAMInletRule_H

#include <logMe/dtMacros.h>
#include "openFOAMSetupRule.h"

namespace dtOO {
  class openFOAMInletRule : public openFOAMSetupRule {
  public:
    dt__class(openFOAMInletRule, openFOAMSetupRule);  
    openFOAMInletRule();
    virtual ~openFOAMInletRule();
    virtual void executeOnMesh(
      std::vector< std::string > const & rule, ::Foam::polyMesh & mesh
    ) const;    
  private:

  };
}
#endif	/* openFOAMInletRule_H */

