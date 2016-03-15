#ifndef OpenFOAMCyclicGgiRule_H
#define	OpenFOAMCyclicGgiRule_H

#include <logMe/dtMacros.h>
#include "OpenFOAMSetupRule.h"

namespace dtOO {
  class OpenFOAMCyclicGgiRule : public OpenFOAMSetupRule {
  public:
    dt__class(OpenFOAMCyclicGgiRule, OpenFOAMSetupRule);  
    OpenFOAMCyclicGgiRule();
    virtual ~OpenFOAMCyclicGgiRule();
    virtual void executeOnMesh(
      std::vector< std::string > const & rule, ::Foam::polyMesh & mesh
    ) const;
    virtual void executeOnVolVectorField(
      std::vector< std::string > const & rule, ::Foam::volVectorField & field
    ) const;    
    virtual void executeOnVolScalarField(
      std::vector< std::string > const & rule, ::Foam::volScalarField & field
    ) const;       
  private:

  };
}
#endif	/* OpenFOAMCyclicGgiRule_H */

