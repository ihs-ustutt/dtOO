#ifndef ofOpenFOAMEmptyRule_H
#define	ofOpenFOAMEmptyRule_H

#include <dtOOTypeDef.h>

#include <logMe/dtMacros.h>
#include "ofOpenFOAMSetupRule.h"

namespace dtOO {
  class ofOpenFOAMEmptyRule : public ofOpenFOAMSetupRule {
    public:
      dt__class(ofOpenFOAMEmptyRule, ofOpenFOAMSetupRule);  
      dt__classSelfCreate(ofOpenFOAMEmptyRule);
      ofOpenFOAMEmptyRule();
      virtual ~ofOpenFOAMEmptyRule();
      virtual void executeOnMesh(
        std::vector< std::string > const & rule, ::Foam::polyMesh & mesh
      ) const;
    private:
      static bool _registrated;

  };
}
#endif	/* ofOpenFOAMEmptyRule_H */

