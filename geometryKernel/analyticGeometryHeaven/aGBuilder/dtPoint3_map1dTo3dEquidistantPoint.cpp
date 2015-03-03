#include "dtPoint3_map1dTo3dEquidistantPoint.h"

#include <analyticGeometryHeaven/map1dTo3d.h>

namespace dtOO {
	dtPoint3_map1dTo3dEquidistantPoint::dtPoint3_map1dTo3dEquidistantPoint(
    map1dTo3d const * const m1d, int const & nPoints
	) {
		std::vector< dtPoint3 > ppXYZ(nPoints);
		float dist = 1./(nPoints-1);
		float length = m1d->length();
		for (int ii=0; ii<nPoints; ii++) {
			dt__TOFLOAT(float iiF, ii);
			_ppXYZ[ii] = m1d->getPointPercent( m1d->u_l(length * dist * iiF) );
		}
	}

	dtPoint3_map1dTo3dEquidistantPoint::~dtPoint3_map1dTo3dEquidistantPoint() {
	}
	
	std::vector< dtPoint3 > dtPoint3_map1dTo3dEquidistantPoint::result( void ) {
		return _ppXYZ;
	}
}
