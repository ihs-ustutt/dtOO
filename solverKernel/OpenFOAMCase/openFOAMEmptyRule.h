#ifndef openFOAMEmptyRule_H
#define	openFOAMEmptyRule_H

#include <logMe/dtMacros.h>
#include "openFOAMSetupRule.h"

namespace dtOO {
  class openFOAMEmptyRule : public openFOAMSetupRule {
  public:
    dt__class(openFOAMEmptyRule, openFOAMSetupRule);  
    openFOAMEmptyRule();
    virtual ~openFOAMEmptyRule();
    virtual void executeOnMesh(
      std::vector< std::string > const & rule, ::Foam::polyMesh & mesh
    ) const;
  private:

  };
}
#endif	/* openFOAMEmptyRule_H */

