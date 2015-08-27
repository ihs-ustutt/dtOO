#ifndef openFOAMCyclicGgiRule_H
#define	openFOAMCyclicGgiRule_H

#include <logMe/dtMacros.h>
#include "openFOAMSetupRule.h"

namespace dtOO {
  class openFOAMCyclicGgiRule : public openFOAMSetupRule {
  public:
    dt__class(openFOAMCyclicGgiRule, openFOAMSetupRule);  
    openFOAMCyclicGgiRule();
    virtual ~openFOAMCyclicGgiRule();
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
#endif	/* openFOAMCyclicGgiRule_H */

