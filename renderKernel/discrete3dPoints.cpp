#include "discrete3dPoints.h"

#include <logMe/logMe.h>
#include <interfaceHeaven/ptrHandling.h>
#include <interfaceHeaven/staticPropertiesHandler.h>

namespace dtOO {
	discrete3dPoints::discrete3dPoints() : renderInterface() {
	}

	discrete3dPoints::~discrete3dPoints() {
	}

  discrete3dPoints::discrete3dPoints(vectorHandling< dtPoint3 > const & pp) : renderInterface() {
    addPoints(pp);
	}
	
	void discrete3dPoints::addPoints( vectorHandling< dtPoint3 > const & pp ) {
		dt__throwIf(pp.size()==0, addPoints());
		
		_p3 = pp;
		
		std::pair<dtPoint3, dtPoint3> minMax = dtLinearAlgebra::boundingBox(_p3);
		setMin( 0, minMax.first.x() );
		setMin( 1, minMax.first.y() );
		setMin( 2, minMax.first.z() );
		setMax( 0, minMax.second.x() );
		setMax( 1, minMax.second.y() );
		setMax( 2, minMax.second.z() );		
	}	
	
	vectorHandling< dtPoint3 > const & discrete3dPoints::refP3( void ) const {
		return _p3;
	}	
}
