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
    DTLOGCHAP(dumping);
    DTINFOWF(dump(), << "label = " << getLabel());		
	}

	bool analyticFunction::isCompound( void ) const {
		return false;
	}

	vectorHandling< analyticFunction const * > analyticFunction::compoundInternal( void ) const {
		return vectorHandling< analyticFunction const * >();
	}			
}
