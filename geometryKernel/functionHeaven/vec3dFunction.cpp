#include "vec3dFunction.h"
#include "dtLinearAlgebra.h"

namespace dtOO {
	vec3dFunction::vec3dFunction() : analyticFunction() {
	}

	vec3dFunction::vec3dFunction(const vec3dFunction& orig) : analyticFunction(orig) {
	}

	vec3dFunction::~vec3dFunction() {
	}
	
  dtPoint3 vec3dFunction::YdtPoint3(aFX const & xx) const {
		aFY yy = Y(xx);
		
		return dtPoint3(yy[0], yy[1], yy[2]);		
	}
}
