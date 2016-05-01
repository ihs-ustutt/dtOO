#ifndef OpenFOAMGgiRule_H
#define	OpenFOAMGgiRule_H

#include <logMe/dtMacros.h>
#include "OpenFOAMSetupRule.h"

namespace dtOO {
  class OpenFOAMGgiRule : public OpenFOAMSetupRule {
  public:
    dt__class(OpenFOAMGgiRule, OpenFOAMSetupRule);  
    OpenFOAMGgiRule();
    virtual ~OpenFOAMGgiRule();
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
#endif	/* OpenFOAMGgiRule_H */

