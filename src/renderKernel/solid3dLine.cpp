#include "solid3dLine.h"

#include <logMe/logMe.h>
#include <interfaceHeaven/ptrHandling.h>

namespace dtOO {
	solid3dLine::solid3dLine() {
	}

	solid3dLine::~solid3dLine() {
	}

  solid3dLine::solid3dLine(vectorHandling< dtPoint3 > const & pp) {
		_p3 = pp;
	}
	
	vectorHandling< dtPoint3 > const & solid3dLine::refP3( void ) const {
		return _p3;
	}		
}
