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
		std::pair<dtPoint3, dtPoint3> minMax = dtLinearAlgebra::boundingBox(_p3);
		setMin( 0, minMax.first.x() );
		setMin( 1, minMax.first.y() );
		setMin( 2, minMax.first.z() );
		setMax( 0, minMax.second.x() );
		setMax( 1, minMax.second.y() );
		setMax( 2, minMax.second.z() );		
	}
	
	vectorHandling< dtPoint3 > const & solid3dLine::refP3( void ) const {
		return _p3;
	}		
}
