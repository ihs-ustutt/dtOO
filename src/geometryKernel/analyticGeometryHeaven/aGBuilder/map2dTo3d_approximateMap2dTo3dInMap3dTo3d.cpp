#include "map2dTo3d_approximateMap2dTo3dInMap3dTo3d.h"

#include <analyticGeometryHeaven/map2dTo3d.h>
#include <analyticGeometryHeaven/vec3dTwoDInMap3dTo3d.h>
#include <analyticFunctionHeaven/vec3dSurfaceTwoD.h>
#include <analyticGeometryHeaven/map3dTo3d.h>

#include "dtPoint3_map1dTo3dPoint.h"
#include <analyticGeometryHeaven/map1dTo3d.h>
#include <geometryEngine/dtCurve.h>
#include <geometryEngine/dtSurface.h>
#include <geometryEngine/geoBuilder/bSplineCurve_pointConstructOCC.h>
#include <geometryEngine/geoBuilder/bSplineSurface_skinConstructOCC.h>

namespace dtOO {
	map2dTo3d_approximateMap2dTo3dInMap3dTo3d
	  ::map2dTo3d_approximateMap2dTo3dInMap3dTo3d(
		map2dTo3d const * const m2d, map3dTo3d const * const m3d,
		int const & nU, dtInt const & nV, dtInt const & order
	) {
		vectorHandling< dtCurve const * > cL;
		
		dt__toFloat(dtReal deltaV, 1./(nV-1));
		for (int jj=0; jj<nV; jj++) {
			//
			// get constV segment
			//
			dt__toFloat(dtReal jjF, jj);
			std::vector< dtPoint3 > ppXYZ
			=
			dtPoint3_map1dTo3dPoint(
				dt__tmpPtr(map1dTo3d,  m2d->segmentConstVPercent(jjF*deltaV)), 
				nU
			).result();
			
			//
			// approximate segment in volume
			//
			std::vector< dtPoint3 > ppUVW(ppXYZ.size());
			dt__forAllIndex(ppXYZ, ii) ppUVW[ii] = m3d->reparamInVolume(ppXYZ[ii]);
			
			//
			// add approximation to curves for surface creation
			//
			cL.push_back( bSplineCurve_pointConstructOCC(ppUVW, order).result() );
		}
				
		//
		// create approximated composition of map2dTo3d
		//
		dt__pH(dtSurface) dtS( bSplineSurface_skinConstructOCC(cL).result() );
		_m2d.reset( 
		  new vec3dTwoDInMap3dTo3d(
		    dt__tmpPtr(
		      vec3dSurfaceTwoD, 
					new vec3dSurfaceTwoD( dtS.get() )
		    ), 
				m3d
		  )
		);
		
		//
		// free memory
		//
		cL.destroy();
	}

	map2dTo3d_approximateMap2dTo3dInMap3dTo3d::~map2dTo3d_approximateMap2dTo3dInMap3dTo3d() {
	}
	
	map2dTo3d * map2dTo3d_approximateMap2dTo3dInMap3dTo3d::result( void ) {
		return _m2d->clone();
	}
}
