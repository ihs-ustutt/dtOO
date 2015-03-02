#include "intHandling.h"

#include <logMe/logMe.h>

#include <boost/math/special_functions/round.hpp>

namespace dtOO {  
  intHandling::intHandling() {
  }

  intHandling::~intHandling() {
  }
  
  int intHandling::round( float a ) {
   return boost::math::iround(a);
	}
}
