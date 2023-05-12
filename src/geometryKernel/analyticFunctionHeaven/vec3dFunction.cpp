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
