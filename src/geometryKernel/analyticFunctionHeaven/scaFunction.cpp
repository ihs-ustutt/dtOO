#include "scaFunction.h"
#include <logMe/logMe.h>
#include <logMe/dtMacros.h>

namespace dtOO {
  scaFunction::scaFunction() : analyticFunction() {
  }

  scaFunction::scaFunction( scaFunction const & orig) : analyticFunction(orig) {
  }

  scaFunction::~scaFunction() {
  }
	
  int scaFunction::yDim( void ) const {
		return 1;
	}  
    
	aFY scaFunction::Y( aFX const & xx ) const {
		aFY yy(1, 0.); 
		yy[0] = YFloat(xx);
		
		return yy;
	}
}