#include "intHandling.h"

#include <logMe/logMe.h>

#include <boost/math/special_functions/round.hpp>

namespace dtOO {  
  intHandling::intHandling() {
  }

  intHandling::~intHandling() {
  }
  
  dtInt intHandling::round( dtReal a ) {
   return boost::math::iround(a);
	}
  
  dtInt intHandling::bound( dtInt const & toBound, dtInt const & aa, dtInt const & bb ) {
    if (toBound < aa) return aa;
    else if (toBound > bb) return bb;
    else return toBound;
  }
}
