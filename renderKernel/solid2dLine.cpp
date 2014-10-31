#include "solid2dLine.h"

#include <logMe/logMe.h>
#include <interfaceHeaven/ptrHandling.h>

namespace dtOO {
	solid2dLine::solid2dLine() : discrete2dPoints() {
	}

	solid2dLine::~solid2dLine() {
	}

  solid2dLine::solid2dLine(vectorHandling< dtPoint2 > const & pp) : discrete2dPoints(pp) {
	}
}
