#ifndef OpenFOAMMixingPlaneRule_H
#define	OpenFOAMMixingPlaneRule_H

#include <logMe/dtMacros.h>
#include "OpenFOAMSetupRule.h"

namespace dtOO {
  class OpenFOAMMixingPlaneRule : public OpenFOAMSetupRule {
  public:
    dt__class(OpenFOAMMixingPlaneRule, OpenFOAMSetupRule);  
    OpenFOAMMixingPlaneRule();
    virtual ~OpenFOAMMixingPlaneRule();
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
#endif	/* OpenFOAMMixingPlaneRule_H */

