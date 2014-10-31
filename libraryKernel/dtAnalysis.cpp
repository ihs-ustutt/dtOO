#include "dtAnalysis.h"
#include <logMe/logMe.h>

#include <RConfigure.h>
#include <Math/GSLMinimizer.h>
#include <Math/Functor.h>
#include <Rtypes.h>

namespace dtOO {
	dtAnalysis::dtAnalysis() {
	}

	dtAnalysis::dtAnalysis(const dtAnalysis& orig) {
	}

	dtAnalysis::~dtAnalysis() {
	}
	
	dtMinimizer * dtAnalysis::createMinimizer( ptrHandling< dtMultiFunction > const & fun ) const {
		dtMinimizer * ptr = new ROOT::Math::GSLMinimizer( ROOT::Math::kVectorBFGS );
		
		ptr->SetFunction(*fun);
		return ptr;
	}
	
	dtMultiFunction * dtAnalysis::createFunction( double (*fPtr)( double const * xx ), int const & dim ) const {
		return new ROOT::Math::Functor(fPtr, dim);
	}
}
