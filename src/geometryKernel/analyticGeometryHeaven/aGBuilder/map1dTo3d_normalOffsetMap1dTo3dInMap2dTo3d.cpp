#include "map1dTo3d_normalOffsetMap1dTo3dInMap2dTo3d.h"
#include <analyticFunctionHeaven/vec2dOneD.h>
#include <analyticFunctionHeaven/vec2dCurve2dOneD.h>
#include <geometryEngine/dtCurve2d.h>

#include <interfaceHeaven/twoDArrayHandling.h>
#include <analyticGeometryHeaven/map1dTo3d.h>
#include <analyticGeometryHeaven/map2dTo3d.h>
#include <analyticFunctionHeaven/vec2dOneD.h>
#include <geometryEngine/geoBuilder/bSplineCurve2d_pointConstructOCC.h>
#include <analyticFunctionHeaven/aFBuilder/float_scaOneDPoint.h>
#include <analyticFunctionHeaven/vec2dCurve2dOneD.h>
#include <analyticFunctionHeaven/scaCurve2dOneD.h>
#include <analyticGeometryHeaven/vec2dOneDInMap2dTo3d.h>
#include "dtPoint3_map1dTo3dPoint.h"
#include "dtPoint3_map1dTo3dEquidistantPoint.h"

namespace dtOO {
	map1dTo3d_normalOffsetMap1dTo3dInMap2dTo3d
	  ::map1dTo3d_normalOffsetMap1dTo3dInMap2dTo3d(
      map1dTo3d const * const m1d, map2dTo3d const * const m2d,
			dtReal const & thick, dtInt const & nPoints, dtInt const & nIntegrationPoints,
		  dtInt const & order
	) {				  
		//
		// should be at least 2 points
		//
		int nPointsMax = std::max(nPoints, 2);
		dt__warnIfWithMessage(
			nPoints!=nPointsMax, 
			map1dTo3d_normalOffsetMap1dTo3dInMap2dTo3d,
			<< "Reset nPoints from " << nPoints << " to " << nPointsMax
		);
		
		std::vector< dtPoint3 > ppXYZ
		= 
		dtPoint3_map1dTo3dEquidistantPoint(m1d, nPointsMax).result();
		std::vector< dtPoint2 > ppNewUV(ppXYZ.size());
		std::vector< dtReal > itVal;
		dtReal deltaThick = thick/nIntegrationPoints;			
		dt__forAllIndex(ppXYZ, ii) {
			dtReal percent 
			= 
			static_cast< dtReal >(ii) / static_cast< dtReal >(nPointsMax-1);
			//
			// reparam on surface and get derivative
			//
			dtPoint2 ppUV = m2d->reparamOnFace( ppXYZ[ii] );				
			dtVector3 dCdU = m1d->firstDerU( *m1d & percent );

			std::vector< dtPoint3 > movingXYZ(2);
			movingXYZ[0] = ppXYZ[ii];
			ppNewUV[ii] = ppUV;
			dtReal intDist = 0.;
			for (int jj=0; jj<nIntegrationPoints; jj++) {
				//
				// approximate normal
				// calculate cross product of surface normal and curve normal
				//
				dtVector3 nS = m2d->normal( ppNewUV[ii] );
				dtVector3 nInC = dtLinearAlgebra::crossProduct(dCdU, nS);					
				nInC = dtLinearAlgebra::normalize(nInC);
				
				//
				// calculate distance with approximated normal
				//
				dtPoint3 ppOpt = movingXYZ[0] + deltaThick*nInC;
				dtVector3 deltaXYZ = ppOpt - movingXYZ[0];
				
				//
				// calculate approximation of distance in parameter space
				// solve: deltaXYZ = J(u,v) deltaUV
				//
				dtVector2 deltaUV 
				= 
				dtLinearAlgebra::toDtVector2(
					dtLinearAlgebra::solveMatrix(
						m2d->jacobi(ppNewUV[ii]), 
						dtLinearAlgebra::createMatrixVector(deltaXYZ)
					)
				);
				
				//
				// update ppNewUV, movingXYZ and calculate discrete distance
				//
				ppNewUV[ii] = ppNewUV[ii] + deltaUV;
				movingXYZ[1] = m2d->getPoint(ppNewUV[ii]);
				intDist = intDist + dtLinearAlgebra::length(movingXYZ[1]-movingXYZ[0]);
				
				//
				// make current value old
				//
				movingXYZ[0] = movingXYZ[1];
			}
			
			//
			// save iteration output
			//
			itVal.push_back( ppUV.x() );
			itVal.push_back( ppUV.y() );
			itVal.push_back( ppNewUV[ii].x() );
			itVal.push_back( ppNewUV[ii].y() );				
			itVal.push_back( intDist );
			itVal.push_back( fabs( (thick-intDist)/thick ) );
		}
		//
		// output
		//
		std::vector< std::string > header;
		header.push_back("p_u");
		header.push_back("p_v");
		header.push_back("pT_u");
		header.push_back("pT_v");			
		header.push_back("int(T)");
		header.push_back("|(T-int(T))/T|");
		dt__info(
			map1dTo3d_normalOffsetMap1dTo3dInMap2dTo3d(),
			<< logMe::vecToTable(header, itVal)
		);			
		
		//
		// create mapping
		//
		_m1d.reset( 
			new vec2dOneDInMap2dTo3d(
				dt__tmpPtr(
					vec2dCurve2dOneD, 
					new vec2dCurve2dOneD(
						dt__tmpPtr(
							dtCurve2d, 
							bSplineCurve2d_pointConstructOCC(ppNewUV, order).result()
						)
					)
		    ),
				m2d
		  )
		);		
	}

	map1dTo3d_normalOffsetMap1dTo3dInMap2dTo3d::~map1dTo3d_normalOffsetMap1dTo3dInMap2dTo3d() {
	}
	
	map1dTo3d * map1dTo3d_normalOffsetMap1dTo3dInMap2dTo3d::result( void ) {
		return _m1d->clone();
	}
}
