#ifndef ofOpenFOAMCyclicAmiRule_H
#define	ofOpenFOAMCyclicAmiRule_H

#include <logMe/dtMacros.h>
#include "ofOpenFOAMSetupRule.h"

namespace dtOO {
  class ofOpenFOAMCyclicAmiRule : public ofOpenFOAMSetupRule {
    public:
      dt__class(ofOpenFOAMCyclicAmiRule, ofOpenFOAMSetupRule);  
      dt__classSelfCreate(ofOpenFOAMCyclicAmiRule);
      ofOpenFOAMCyclicAmiRule();
      virtual ~ofOpenFOAMCyclicAmiRule();
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
#endif	/* ofOpenFOAMCyclicAmiRule_H */

