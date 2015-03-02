#include "map2dTo3d_constructMarginFaces.h"

#include <analyticGeometryHeaven/map2dTo3d.h>

namespace dtOO {
	map2dTo3d_constructMarginFaces::map2dTo3d_constructMarginFaces(
	  map2dTo3d const * const m2d, float const & width
	) {
		std::vector< dtPoint2 > embVertUV(4);
		embVertUV[0] = dtPoint2(width, width);
		embVertUV[1] = dtPoint2(1.-width, width);
		embVertUV[2] = dtPoint2(1.-width, 1.-width);
		embVertUV[3] = dtPoint2(width, 1.-width);
		
    _m2dV.push_back(
		  m2d->segmentPercent(embVertUV[0], embVertUV[1], embVertUV[2], embVertUV[3])
		);
		_m2dV.push_back(
		  m2d->segmentPercent(dtPoint2(0, 0), dtPoint2(1, 0), embVertUV[1], embVertUV[0])
		);
		_m2dV.push_back(
		  m2d->segmentPercent(dtPoint2(1, 0), dtPoint2(1, 1), embVertUV[2], embVertUV[1])
		);
		_m2dV.push_back(
		  m2d->segmentPercent(dtPoint2(1, 1), dtPoint2(0, 1), embVertUV[3], embVertUV[2])
		);
		_m2dV.push_back(
		  m2d->segmentPercent(dtPoint2(0, 1), dtPoint2(0, 0), embVertUV[0], embVertUV[3])
		);		
	}

	map2dTo3d_constructMarginFaces::~map2dTo3d_constructMarginFaces() {
	}
	
	dt__pVH(map2dTo3d) map2dTo3d_constructMarginFaces::result( void ) {
		return _m2dV;
	}
}
