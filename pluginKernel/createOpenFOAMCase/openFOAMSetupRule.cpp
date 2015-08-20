#include "openFOAMSetupRule.h"

#include <logMe/logMe.h>
#include "openFOAMWallRule.h"
#include "openFOAMGgiRule.h"

namespace dtOO {
  openFOAMSetupRule::openFOAMSetupRule() {
  }

  openFOAMSetupRule::~openFOAMSetupRule() {
  }
  
  openFOAMSetupRule * openFOAMSetupRule::create( 
    std::string const & name 
  ) {
    if (name == "openFOAMWallRule") return new openFOAMWallRule();
    if (name == "openFOAMGgiRule") return new openFOAMGgiRule();
    
    
    dt__throw(create(), << "Cannot create " << name);
  }
}
