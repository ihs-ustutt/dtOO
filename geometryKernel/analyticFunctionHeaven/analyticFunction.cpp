#include "analyticFunction.h"

#include <logMe/logMe.h>

namespace dtOO {
	analyticFunction::analyticFunction() : optionHandling(), labelHandling() {
	}

	analyticFunction::analyticFunction(const analyticFunction& orig)  : optionHandling(orig), labelHandling(orig) {
	}

	analyticFunction::~analyticFunction() {
	}
	
  void analyticFunction::dump(void) const {
    dt__info(
			dump(), 
			<< dt__eval(getLabel()) << std::endl
		  << dt__eval(virtualClassName()) << std::endl
			<< "dumpToString()" << std::endl
			<< dumpToString()
		);		
	}
	
	std::string analyticFunction::dumpToString(void) const {
		return std::string("");
	}	

	bool analyticFunction::isCompound( void ) const {
		return false;
	}

	vectorHandling< analyticFunction const * > analyticFunction::compoundInternal( void ) const {
		return vectorHandling< analyticFunction const * >();
	}			
	
  aFX analyticFunction::aFXOneD( float const & x0 ) {
		aFX ret = aFX(1, 0.);
		ret[0] = x0;
		
		return ret;
	}
	
  aFX analyticFunction::aFXTwoD( float const & x0, float const & x1 ) {
		aFX ret = aFX(2, 0.);
		ret[0] = x0;
		ret[1] = x1;		
		
		return ret;
	}
	
  aFX analyticFunction::aFXThreeD( float const & x0, float const & x1, float const & x2 ) {
		aFX ret = aFX(3, 0.);
		ret[0] = x0;
		ret[1] = x1;
		ret[2] = x2;	
		
		return ret;
	}
}
