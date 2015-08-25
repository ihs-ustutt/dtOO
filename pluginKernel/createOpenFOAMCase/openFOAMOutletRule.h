#ifndef openFOAMOutletRule_H
#define	openFOAMOutletRule_H

#include <logMe/dtMacros.h>
#include "openFOAMSetupRule.h"

namespace dtOO {
  class openFOAMOutletRule : public openFOAMSetupRule {
  public:
    dt__class(openFOAMOutletRule, openFOAMSetupRule);  
    openFOAMOutletRule();
    virtual ~openFOAMOutletRule();
    virtual void executeOnMesh(
      std::vector< std::string > const & rule, ::Foam::polyMesh & mesh
    ) const;    
  private:

  };
}
#endif	/* openFOAMOutletRule_H */

