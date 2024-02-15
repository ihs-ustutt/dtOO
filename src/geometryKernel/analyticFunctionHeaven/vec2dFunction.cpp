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

#include "vec2dFunction.h"
#include "dtLinearAlgebra.h"

namespace dtOO {
	vec2dFunction::vec2dFunction() : analyticFunction() {
	}

	vec2dFunction::vec2dFunction(const vec2dFunction& orig) : analyticFunction(orig) {
	}

	vec2dFunction::~vec2dFunction() {
	}
	
	int vec2dFunction::yDim( void ) const {
		return 2;
	}  
  
  dtPoint2 vec2dFunction::YdtPoint2(aFX const & xx) const {
		aFY yy = Y(xx);
		
		return dtPoint2(yy[0], yy[1]);		
	}
  dt__C_addCloneForpVH(vec2dFunction);
}
