#include "dtPoint3_map1dTo3dPoint.h"

#include <analyticGeometryHeaven/map1dTo3d.h>

namespace dtOO {
	dtPoint3_map1dTo3dPoint::dtPoint3_map1dTo3dPoint(
    map1dTo3d const * const m1d, int const & nPoints
	) {
		_ppXYZ.resize(nPoints);
		float dist = 1./(nPoints-1);
		for (int ii=0; ii<nPoints; ii++) {
			dt__toFloat(float iiF, ii);
			_ppXYZ[ii] = m1d->getPoint( *m1d % (dist*iiF) );
		}
	}

	dtPoint3_map1dTo3dPoint::~dtPoint3_map1dTo3dPoint() {
	}
	
	std::vector< dtPoint3 > dtPoint3_map1dTo3dPoint::result( void ) {
		return _ppXYZ;
	}
}
