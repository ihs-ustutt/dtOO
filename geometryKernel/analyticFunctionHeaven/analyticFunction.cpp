#include "analyticFunction.h"

#include <logMe/logMe.h>

namespace dtOO {
	analyticFunction::analyticFunction() : optionHandling(), labelHandling() {
	}

	analyticFunction::analyticFunction(const analyticFunction& orig)  : optionHandling(orig), labelHandling(orig) {
	}

	analyticFunction::~analyticFunction() {
	}
	
  void analyticFunction::dump(void) const {
    dt__info(
			dump(), 
			<< dt__eval(getLabel()) << std::endl
		  << dt__eval(virtualClassName()) << std::endl
			<< "dumpToString()" << std::endl
			<< dumpToString()
		);		
	}
	
	std::string analyticFunction::dumpToString(void) const {
		return std::string("");
	}	
  
  aFX analyticFunction::invY(aFY const & yy) const {
    dt__throw(invY(), << "Call on base class.");
  }

	bool analyticFunction::isCompound( void ) const {
		return false;
	}

	vectorHandling< analyticFunction const * > analyticFunction::compoundInternal( void ) const {
		return vectorHandling< analyticFunction const * >();
	}			
  
	bool analyticFunction::isTransformed( void ) const {
		return false;
	}
	
  aFX analyticFunction::aFXZeroD( void ) {
		return aFX(0);
	}
  
  aFX analyticFunction::aFXOneD( float const & x0 ) {
		aFX ret = aFX(1, 0.);
		ret[0] = x0;
		
		return ret;
	}
	
  aFX analyticFunction::aFXTwoD( float const & x0, float const & x1 ) {
		aFX ret = aFX(2, 0.);
		ret[0] = x0;
		ret[1] = x1;		
		
		return ret;
	} 
	
  aFX analyticFunction::aFXThreeD( float const & x0, float const & x1, float const & x2 ) {
		aFX ret = aFX(3, 0.);
		ret[0] = x0;
		ret[1] = x1;
		ret[2] = x2;	
		
		return ret;
	}
  
  aFX analyticFunction::aFXTwoD( dtPoint2 const & pp ) {
		return analyticFunction::aFXTwoD(pp.x(), pp.y());
	} 
  
  aFX analyticFunction::aFXThreeD( dtPoint3 const & pp ) {
		return analyticFunction::aFXThreeD(pp.x(), pp.y(), pp.z());
	}  
  
  aFY analyticFunction::aFYZeroD( void ) {
		return aFY(0);
	}
  
  aFY analyticFunction::aFYOneD( float const & x0 ) {
		aFY ret = aFY(1, 0.);
		ret[0] = x0;
		
		return ret;
	}
	
  aFY analyticFunction::aFYTwoD( float const & x0, float const & x1 ) {
		aFY ret = aFY(2, 0.);
		ret[0] = x0;
		ret[1] = x1;		
		
		return ret;
	}
	
  aFY analyticFunction::aFYThreeD( float const & x0, float const & x1, float const & x2 ) {
		aFY ret = aFY(3, 0.);
		ret[0] = x0;
		ret[1] = x1;
		ret[2] = x2;	
		
		return ret;
	}  
  
  aFY analyticFunction::aFYTwoD( dtPoint2 const & pp ) {
		return analyticFunction::aFYTwoD(pp.x(), pp.y());
	} 
  
  aFY analyticFunction::aFYThreeD( dtPoint3 const & pp ) {
		return analyticFunction::aFYThreeD(pp.x(), pp.y(), pp.z());
	}    
}
