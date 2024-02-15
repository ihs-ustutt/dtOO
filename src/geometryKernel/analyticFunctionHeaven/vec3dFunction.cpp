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

#include "vec3dFunction.h"
#include "dtLinearAlgebra.h"

namespace dtOO {
	vec3dFunction::vec3dFunction() : analyticFunction() {
	}

	vec3dFunction::vec3dFunction(const vec3dFunction& orig) : analyticFunction(orig) {
	}

	vec3dFunction::~vec3dFunction() {
	}
	
	int vec3dFunction::yDim( void ) const {
		return 3;
	}  
  
  dtPoint3 vec3dFunction::YdtPoint3(aFX const & xx) const {
		aFY yy = Y(xx);
		
		return dtPoint3(yy[0], yy[1], yy[2]);		
	}
  dt__C_addCloneForpVH(vec3dFunction);
}
