/*---------------------------------------------------------------------------*\
  dtOO < design tool Object-Oriented >
    
    Copyright (C) 2024 A. Tismer.
-------------------------------------------------------------------------------
License
    This file is part of dtOO.

    dtOO is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the LICENSE.txt file in the
    dtOO root directory for more details.

    You should have received a copy of the License along with dtOO.

\*---------------------------------------------------------------------------*/

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
