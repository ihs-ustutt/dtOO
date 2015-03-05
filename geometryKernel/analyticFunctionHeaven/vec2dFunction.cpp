#include "vec2dFunction.h"
#include "dtLinearAlgebra.h"

namespace dtOO {
	vec2dFunction::vec2dFunction() : analyticFunction() {
	}

	vec2dFunction::vec2dFunction(const vec2dFunction& orig) : analyticFunction(orig) {
	}

	vec2dFunction::~vec2dFunction() {
	}
	
  dtPoint2 vec2dFunction::YdtPoint2(aFX const & xx) const {
		aFY yy = Y(xx);
		
		return dtPoint2(yy[0], yy[1]);		
	}
}
