#ifndef OpenFOAMEmptyRule_H
#define	OpenFOAMEmptyRule_H

#include <logMe/dtMacros.h>
#include "OpenFOAMSetupRule.h"

namespace dtOO {
  class OpenFOAMEmptyRule : public OpenFOAMSetupRule {
  public:
    dt__class(OpenFOAMEmptyRule, OpenFOAMSetupRule);  
    OpenFOAMEmptyRule();
    virtual ~OpenFOAMEmptyRule();
    virtual void executeOnMesh(
      std::vector< std::string > const & rule, ::Foam::polyMesh & mesh
    ) const;
  private:

  };
}
#endif	/* OpenFOAMEmptyRule_H */

