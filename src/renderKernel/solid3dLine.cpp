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

#include "solid3dLine.h"

#include <logMe/logMe.h>
#include <interfaceHeaven/ptrHandling.h>

namespace dtOO {
	solid3dLine::solid3dLine() {
	}

	solid3dLine::~solid3dLine() {
	}

  solid3dLine::solid3dLine(std::vector< dtPoint3 > const & pp) {
		_p3 = pp;
	}
	
	std::vector< dtPoint3 > const & solid3dLine::refP3( void ) const {
		return _p3;
	}		
}
