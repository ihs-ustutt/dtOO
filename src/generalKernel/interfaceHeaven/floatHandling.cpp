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

  float floatHandling::nextIfSmaller(double a) {
    float b = a;
    if ( b >= a ) {
      return b;
    }
    else {
      return next(a);
    }
  }  
  
  float floatHandling::prev(double a) {
    float b = a;
    if ( (double)b < a ) {
      return b;
    }
    
    float ret = nexttowardf(b, -std::numeric_limits<float>::infinity());
    return ret;
  }  
  
  float floatHandling::prevIfBigger(double a) {
    float b = a;
    if ( b <= a ) {
      return b;
    }
    else {
      return prev(a);
    }
  }  
  
  float floatHandling::boundToRange(float const value, float const a, float const b) {
    if (value > b) {
      return b;
    }
    else if (value < a) {
      return a;
    }
    
    return value;
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
		if ( fabs(val) < dt__SMALL ) {
			return true;
		}
		return false;
	}
}
