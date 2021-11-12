#ifndef ofOpenFOAMWallRule_H
#define	ofOpenFOAMWallRule_H

#include <logMe/dtMacros.h>
#include "ofOpenFOAMSetupRule.h"

namespace dtOO {
  class ofOpenFOAMWallRule : public ofOpenFOAMSetupRule {
    public:
      dt__class(ofOpenFOAMWallRule, ofOpenFOAMSetupRule);  
      dt__classSelfCreate(ofOpenFOAMWallRule);
      ofOpenFOAMWallRule();
      virtual ~ofOpenFOAMWallRule();
      virtual std::vector< std::string > factoryAlias( void ) const;
      virtual void executeOnMesh(
        std::vector< std::string > const & rule, ::Foam::polyMesh & mesh
      ) const;       
    private:
      static bool _registrated;
  };
}
#endif	/* ofOpenFOAMWallRule_H */

