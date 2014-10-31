#include "solid3dSurface.h"

#include <logMe/logMe.h>
#include <interfaceHeaven/ptrHandling.h>

namespace dtOO {
	solid3dSurface::solid3dSurface() {
	}

	solid3dSurface::~solid3dSurface() {
	}

	/**
   * @todo: Set min max automatically.
   */
  solid3dSurface::solid3dSurface(twoDArrayHandling< dtPoint3 > const & pp) {
		_p3 = pp;
//		std::pair<dtPoint3, dtPoint3> minMax = dtLinearAlgebra::boundingBox(_p3);
//		setMin( 0, minMax.first.x() );
//		setMin( 1, minMax.first.y() );
//		setMin( 2, minMax.first.z() );
//		setMax( 0, minMax.second.x() );
//		setMax( 1, minMax.second.y() );
//		setMax( 2, minMax.second.z() );		
	}
	
	twoDArrayHandling< dtPoint3 > const solid3dSurface::refP3( void ) const {
		return _p3;
	}
}
