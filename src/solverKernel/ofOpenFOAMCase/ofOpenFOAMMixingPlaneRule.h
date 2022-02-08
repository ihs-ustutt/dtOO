#ifndef ofOpenFOAMMixingPlaneRule_H
#define	ofOpenFOAMMixingPlaneRule_H

#include <dtOOTypeDef.h>

#include <logMe/dtMacros.h>
#include "ofOpenFOAMSetupRule.h"

namespace dtOO {
  class ofOpenFOAMMixingPlaneRule : public ofOpenFOAMSetupRule {
    public:
      dt__class(ofOpenFOAMMixingPlaneRule, ofOpenFOAMSetupRule);  
      dt__classSelfCreate(ofOpenFOAMMixingPlaneRule);
      ofOpenFOAMMixingPlaneRule();
      virtual ~ofOpenFOAMMixingPlaneRule();
      virtual std::vector< std::string > factoryAlias( void ) const;
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
      static bool _registrated;
  };
}
#endif	/* ofOpenFOAMMixingPlaneRule_H */

