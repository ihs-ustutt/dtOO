/*---------------------------------------------------------------------------*\
  dtOO < design tool Object-Oriented >
    
    Copyright (C) 2024 A. Tismer.
-------------------------------------------------------------------------------
License
    This file is part of dtOO.

    dtOO is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the LICENSE.txt file in the
    dtOO root directory for more details.

    You should have received a copy of the License along with dtOO.

\*---------------------------------------------------------------------------*/

#include "discrete3dPoints.h"

#include <logMe/logMe.h>
#include <interfaceHeaven/ptrHandling.h>
#include <interfaceHeaven/staticPropertiesHandler.h>

namespace dtOO {
	discrete3dPoints::discrete3dPoints() : renderInterface() {
	}

	discrete3dPoints::~discrete3dPoints() {
	}

  discrete3dPoints::discrete3dPoints(
    std::vector< dtPoint3 > const & pp
  ) : renderInterface() {
    addPoints(pp);
	}
	
	void discrete3dPoints::addPoints( std::vector< dtPoint3 > const & pp ) {
		dt__throwIf(pp.size()==0, addPoints());
		
		_p3 = pp;
		
		std::pair<dtPoint3, dtPoint3> minMax = dtLinearAlgebra::boundingBox(_p3);
		geoBoundMin( 0, minMax.first.x() );
		geoBoundMin( 1, minMax.first.y() );
		geoBoundMin( 2, minMax.first.z() );
		geoBoundMax( 0, minMax.second.x() );
		geoBoundMax( 1, minMax.second.y() );
		geoBoundMax( 2, minMax.second.z() );		
	}	
	
	std::vector< dtPoint3 > const & discrete3dPoints::constRefP3( void ) const {
		return _p3;
	}	

	std::vector< dtPoint3 > & discrete3dPoints::refP3( void ) {
		return _p3;
	}	  
}
