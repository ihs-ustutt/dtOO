#include "float_scaOneDPoint.h"

#include <logMe/dtMacros.h>
#include <analyticFunctionHeaven/scaOneD.h>

namespace dtOO {
  float_scaOneDPoint::float_scaOneDPoint(
	  scaOneD const * const s1d, dtInt const & nPoints
  ) {
		_ff.resize(nPoints);
		
		dtReal dist = 1./(nPoints-1);
		for (int ii=0; ii<nPoints; ii++) {
			dt__toFloat(dtReal iiF, ii);
			_ff[ii] = s1d->YFloat( dist * iiF );
		}				
	}
  
	float_scaOneDPoint::~float_scaOneDPoint() {
		
	}
	
	std::vector< dtReal > float_scaOneDPoint::result( void ) {
		return _ff;
	}
}

