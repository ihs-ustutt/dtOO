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
  
  int intHandling::bound( int const & toBound, int const & aa, int const & bb ) {
    if (toBound < aa) return aa;
    else if (toBound > bb) return bb;
    else return toBound;
  }
}
