#ifndef openFOAMCylindricalInletRule_H
#define	openFOAMCylindricalInletRule_H

#include <logMe/dtMacros.h>
#include "openFOAMSetupRule.h"

namespace dtOO {
  class openFOAMCylindricalInletRule : public openFOAMSetupRule {
  public:
    dt__class(openFOAMCylindricalInletRule, openFOAMSetupRule);  
    openFOAMCylindricalInletRule();
    virtual ~openFOAMCylindricalInletRule();
    void executeOnVolVectorField(
      std::vector< std::string > const & rule, ::Foam::volVectorField & field
    ) const;
  private:

  };
}
#endif	/* openFOAMCylindricalInletRule_H */

