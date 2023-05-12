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
