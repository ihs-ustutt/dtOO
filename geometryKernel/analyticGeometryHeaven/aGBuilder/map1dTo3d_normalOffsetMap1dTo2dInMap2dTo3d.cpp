#include "map1dTo3d_normalOffsetMap1dTo2dInMap2dTo3d.h"
#include <functionHeaven/vec2dOneD.h>
#include <functionHeaven/vec2dCurve2dOneD.h>
#include <geometryEngine/dtCurve2d.h>

#include <interfaceHeaven/twoDArrayHandling.h>
#include <analyticGeometryHeaven/map1dTo3d.h>
#include <analyticGeometryHeaven/map2dTo3d.h>
#include <functionHeaven/vec2dOneD.h>
#include <geometryEngine/geoBuilder/bSplineCurve2d_pointConstructOCC.h>
#include <geometryEngine/geoBuilder/bSplineCurve2d_normalOffsetGeomCurve2dOCC.h>
#include <functionHeaven/vec2dCurve2dOneD.h>
#include <functionHeaven/scaCurve2dOneD.h>
#include <analyticGeometryHeaven/vec2dOneDInMap2dTo3d.h>

namespace dtOO {
	map1dTo3d_normalOffsetMap1dTo2dInMap2dTo3d
	  ::map1dTo3d_normalOffsetMap1dTo2dInMap2dTo3d(
      map1dTo3d const * const m1d, float const & thick
	) {
		dt__PTRASS(
			vec2dOneDInMap2dTo3d const * const v3dInM3d,
			vec2dOneDInMap2dTo3d::ConstDownCast(m1d)
		);
		dt__THROW_IF(
		  v3dInM3d->isPercent(), 
			map1dTo3d_normalOffsetMap1dTo2dInMap2dTo3d
		);
		dt__PTRASS(
			vec2dCurve2dOneD const * const v2d1d,
			vec2dCurve2dOneD::ConstDownCast(v3dInM3d->ptrToVec2dOneD())
		);
		map2dTo3d const * const m2d = v3dInM3d->ptrToMap2dTo3d();
		
		std::vector< float > percent(3);
		percent[0] = 0.0;
		percent[1] = 0.5;
		percent[2] = 1.0;
		
		std::vector< dtPoint2 > v2d(percent.size());
		twoDArrayHandling< dtVector3 > dM(percent.size(), 2);
		std::vector< dtVector2 > nV2d(percent.size());
		std::vector< dtPoint2 > thickPoints(percent.size());
		for ( int ii=0; ii<percent.size(); ii++) {
			v2d[ii] = v2d1d->YdtPoint2Percent(percent[ii]);
		  dM[ii] = m2d->firstDer(v2d[ii].x(), v2d[ii].y());
      nV2d[ii] 
			= 
			dtLinearAlgebra::unitNormal(
				v2d1d->DYdtVector2( v2d1d->x_percent(percent[ii]) )
			);			

			dtVector3 gradNormalC = nV2d[ii].x()*dM[ii][0] + nV2d[ii].y()*dM[ii][1];
      thickPoints[ii] 
			= 
			dtPoint2(percent[ii], thick/dtLinearAlgebra::length(gradNormalC));
			DTINFOWF(
				map1dTo3d_normalOffsetMap1dTo2dInMap2dTo3d(),
				<< DTLOGEVAL(percent[ii]) << LOGDEL
				<< DTLOGVEC3D(dM[ii][0]) << LOGDEL
				<< DTLOGVEC3D(dM[ii][1]) << LOGDEL
				<< DTLOGVEC2D(nV2d[ii]) << LOGDEL
				<< DTLOGVEC3D(gradNormalC) << LOGDEL
				<< DTLOGEVAL(dtLinearAlgebra::length(gradNormalC)) << LOGDEL
				<< DTLOGEVAL(thick) << LOGDEL
				<< DTLOGPOI2D(thickPoints[ii])
			);			
    }
		
		scaCurve2dOneD fSmooth(
      dt__tmpPtr(dtCurve2d, bSplineCurve2d_pointConstructOCC(thickPoints, 1).result())						
		);

		int nPoints 
	  = 
		v2d1d->ptrDtCurve2d()->nControlPoints();
		
		std::vector< float > thickSmooth(nPoints);
		float dist = 1./(nPoints-1);
		for (int ii=0; ii<nPoints; ii++) {
			dt__TOFLOAT(float iiF, ii);
			thickSmooth[ii] = fSmooth.YFloat( dist * iiF );
		}		
		
	  
		_m1d.reset( 
		  new vec2dOneDInMap2dTo3d(
				dt__tmpPtr(
		      vec2dCurve2dOneD,
					new vec2dCurve2dOneD(
						dt__tmpPtr(
							dtCurve2d, 
							bSplineCurve2d_normalOffsetGeomCurve2dOCC(
		            v2d1d->ptrDtCurve2d(), thickSmooth
              ).result()
						)
					)
				), 
				v3dInM3d->ptrToMap2dTo3d()
		  ) 
		);
	}

	map1dTo3d_normalOffsetMap1dTo2dInMap2dTo3d::~map1dTo3d_normalOffsetMap1dTo2dInMap2dTo3d() {
	}
	
	map1dTo3d * map1dTo3d_normalOffsetMap1dTo2dInMap2dTo3d::result( void ) {
		return _m1d->clone();
	}
}
