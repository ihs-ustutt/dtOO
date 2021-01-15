#ifndef OpenFOAMCylindricalInletRule_H
#define	OpenFOAMCylindricalInletRule_H

#include <logMe/dtMacros.h>
#include "OpenFOAMSetupRule.h"

namespace dtOO {
  class OpenFOAMCylindricalInletRule : public OpenFOAMSetupRule {
    public:
      dt__class(OpenFOAMCylindricalInletRule, OpenFOAMSetupRule);  
      OpenFOAMCylindricalInletRule();
      virtual ~OpenFOAMCylindricalInletRule();
      void executeOnVolVectorField(
        std::vector< std::string > const & rule, ::Foam::volVectorField & field
      ) const;
    private:

  };
}
#endif	/* OpenFOAMCylindricalInletRule_H */

