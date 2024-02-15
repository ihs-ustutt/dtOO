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

#include "dtPoint3_map2dTo3dPoint.h"

#include <logMe/dtMacros.h>
#include <analyticGeometryHeaven/map2dTo3d.h>

namespace dtOO {
  dtPoint3_map2dTo3dPoint::dtPoint3_map2dTo3dPoint(
      map2dTo3d const * const m2d, dtInt const & nU, dtInt const & nV
  ) {
		_pp.resize(nU, nV);

    twoDArrayHandling< dtPoint2 > uGrid = dtLinearAlgebra::unitGrid(nU, nV);
		dt__forAllIndex(uGrid, ii) {
      dt__forAllIndex(uGrid[ii], jj) {
			  _pp[ii][jj] 
        = 
        m2d->getPointPercent( uGrid[ii][jj] );
      }
		}				
	}
  
	dtPoint3_map2dTo3dPoint::~dtPoint3_map2dTo3dPoint() {
		
	}
	
	twoDArrayHandling< dtPoint3 > dtPoint3_map2dTo3dPoint::result( void ) {
		return _pp;
	}
}
