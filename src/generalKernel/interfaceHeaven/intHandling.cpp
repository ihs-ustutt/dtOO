#include "intHandling.h"

#include <logMe/logMe.h>

#include <boost/math/special_functions/round.hpp>

#include "calculationTypeHandling.h"

namespace dtOO {  
  intHandling::intHandling() {
  }

  intHandling::~intHandling() {
  }
  
  dtInt intHandling::round( dtReal a ) {
    dtInt ret = boost::math::iround(a);

    if ( !floatHandling::isSmall(static_cast< float >(ret)-a) ) {
      dt__warning(
        round(),
        << "Convert dtReal " << a << " to int " << ret << " ." << std::endl
        << "Difference when converting back to dtReal is " 
        << static_cast< float >(ret)-a
      );
    }
    return ret;
	}
  
  dtInt intHandling::bound( 
    dtInt const & toBound, dtInt const & aa, dtInt const & bb 
  ) {
    if (toBound < aa) return aa;
    else if (toBound > bb) return bb;
    else return toBound;
  }
}
