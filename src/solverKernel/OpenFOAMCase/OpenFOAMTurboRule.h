#ifndef OpenFOAMTurboRule_H
#define	OpenFOAMTurboRule_H

#include <logMe/dtMacros.h>
#include "OpenFOAMSetupRule.h"

namespace dtOO {
  class OpenFOAMTurboRule : public OpenFOAMSetupRule {
    public:
      dt__classOnlyName(OpenFOAMTurboRule);    
      OpenFOAMTurboRule();
      virtual ~OpenFOAMTurboRule();
//      virtual void executeOnMesh(
//        std::vector< std::string > const & rule, ::Foam::polyMesh & mesh
//      ) const;
      virtual void executeOnVolVectorField(
        std::vector< std::string > const & rule, ::Foam::volVectorField & field
      ) const;    
      virtual void executeOnVolScalarField(
        std::vector< std::string > const & rule, ::Foam::volScalarField & field
      ) const;
  };
}
#endif	/* OpenFOAMTurboRule_H */

