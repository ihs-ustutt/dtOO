#include "map2dTo3d_constructMarginFaces.h"

#include <analyticGeometryHeaven/map2dTo3d.h>
#include <analyticGeometryHeaven/map1dTo3d.h>

namespace dtOO {
	map2dTo3d_constructMarginFaces::map2dTo3d_constructMarginFaces(
	  map2dTo3d const * const m2d, float const & width
	) {
		//
		// get bounding edges
		//
		dt__pVH(map1dTo3d) m1d;
		m1d.push_back(m2d->segmentConstVPercent(0.));
		m1d.push_back(m2d->segmentConstUPercent(1.));
		m1d.push_back(m2d->segmentConstVPercent(1.));
		m1d.push_back(m2d->segmentConstUPercent(0.));
		
		std::vector< float > ll;
		
		dt__FORALLCONSTITER(dt__pVH(map1dTo3d), m1d, it) {
			map1dTo3d const & thisCurve = *it;
			ll.push_back( thisCurve.length() );
		}
		
    std::vector< dtPoint2 > embVertUV(4);						
		embVertUV[0] = dtPoint2(m1d[0].percent_l(width), m1d[3].percent_l(width));
		embVertUV[1] = dtPoint2(m1d[0].percent_l(ll[0]-width), m1d[1].percent_l(width));
		embVertUV[2] = dtPoint2(m1d[2].percent_l(ll[2]-width), m1d[1].percent_l(ll[1]-width));
		embVertUV[3] = dtPoint2(m1d[2].percent_l(width), m1d[3].percent_l(ll[3]-width));
		
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
