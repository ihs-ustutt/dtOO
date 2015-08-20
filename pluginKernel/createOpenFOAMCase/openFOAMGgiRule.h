#ifndef openFOAMGgiRule_H
#define	openFOAMGgiRule_H

#include <logMe/dtMacros.h>
#include "openFOAMSetupRule.h"

namespace dtOO {
  class openFOAMGgiRule : public openFOAMSetupRule {
  public:
    dt__class(openFOAMGgiRule, openFOAMSetupRule);  
    openFOAMGgiRule();
    virtual ~openFOAMGgiRule();
    virtual void executeOnMesh(
      std::vector< std::string > const & rule, ::Foam::polyMesh & mesh
    ) const;    
  private:

  };
}
#endif	/* openFOAMGgiRule_H */

