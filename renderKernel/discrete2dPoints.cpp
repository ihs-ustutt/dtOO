#include "discrete2dPoints.h"

#include <logMe/logMe.h>
#include <interfaceHeaven/ptrHandling.h>

namespace dtOO {
	discrete2dPoints::discrete2dPoints() : renderInterface() {
	}

	discrete2dPoints::~discrete2dPoints() {
	}

  discrete2dPoints::discrete2dPoints(vectorHandling< dtPoint2 > const & pp) : renderInterface() {
		_p2 = pp;
		std::pair<dtPoint2, dtPoint2> minMax = dtLinearAlgebra::boundingBox(_p2);
		setMin( 0, minMax.first.x() );
		setMin( 1, minMax.first.y() );
		setMax( 0, minMax.second.x() );
		setMax( 1, minMax.second.y() );
		//							<< "S0 LINESTYLE 3\n"
//							<< "SETS LINEWIDTH 2\n"
//							<< "S0 LINEWIDTH 1\n"
//							<< "S0 SYMBOL 2\n";   
//		refOptionHandling().setOption("[covise_0]LINESTYLE", "3");
//		refOptionHandling().setOption("[covise_0]SYMBOL", "2");
	}
	
	vectorHandling< dtPoint2 > const & discrete2dPoints::refP2( void ) const {
		return _p2;
	}
}
