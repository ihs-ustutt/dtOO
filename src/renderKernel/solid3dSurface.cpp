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
//		geoBoundMin( 0, minMax.first.x() );
//		geoBoundMin( 1, minMax.first.y() );
//		geoBoundMin( 2, minMax.first.z() );
//		geoBoundMax( 0, minMax.second.x() );
//		geoBoundMax( 1, minMax.second.y() );
//		geoBoundMax( 2, minMax.second.z() );		
	}
	
	twoDArrayHandling< dtPoint3 > const solid3dSurface::refP3( void ) const {
		return _p3;
	}
}
