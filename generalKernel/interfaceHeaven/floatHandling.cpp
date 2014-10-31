#include "floatHandling.h"

#include <math.h>
#include <limits>

#include <logMe/logMe.h>

#define dt__SMALL +01.E-06

namespace dtOO {  
  floatHandling::floatHandling() {
  }

  floatHandling::~floatHandling() {
  }
  
  float floatHandling::next(double a) {
    float b = a;
    if ( (double)b > a ) {
      return b;
    }
    float ret = nextafterf(b, std::numeric_limits<float>::infinity());
    return ret;
  }
  
  float floatHandling::prev(double a) {
    float b = a;
    if ( (double)b < a ) {
      return b;
    }
    
    float ret = nexttowardf(b, -std::numeric_limits<float>::infinity());
    return ret;
  }  
  
  double floatHandling::boundToRange(float const value, double const a, double const b) {
    double valueD = (double) value;
    
    if (valueD > b) {
      return b;
    }
    else if (valueD < a) {
      return a;
    }
    
    return valueD;
  }
  
  bool floatHandling::isAscending( std::vector< float > const & check) {
    for (int ii=1; ii<check.size(); ii++) {
      if (check[ii-1] > check[ii] ) {
        return false;
      }
    }
    return true;
  }
	
	bool floatHandling::isSmall( float const & val ) {
		DTDEBUGWF(isSmall(),
			<< DTLOGEVAL(dt__SMALL) << LOGDEL
			<< DTLOGEVAL(val) << LOGDEL
		);
		if ( fabs(val) < dt__SMALL ) {
			return true;
		}
		return false;
	}
}
