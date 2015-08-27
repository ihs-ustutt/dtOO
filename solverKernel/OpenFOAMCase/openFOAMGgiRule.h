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
    virtual void executeOnVolVectorField(
      std::vector< std::string > const & rule, ::Foam::volVectorField & field
    ) const;    
    virtual void executeOnVolScalarField(
      std::vector< std::string > const & rule, ::Foam::volScalarField & field
    ) const;         
  private:

  };
}
#endif	/* openFOAMGgiRule_H */

