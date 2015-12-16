#include "dtPoint3_map2dTo3dPoint.h"

#include <logMe/dtMacros.h>
#include <analyticGeometryHeaven/map2dTo3d.h>

namespace dtOO {
  dtPoint3_map2dTo3dPoint::dtPoint3_map2dTo3dPoint(
      map2dTo3d const * const m2d, int const & nU, int const & nV
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

