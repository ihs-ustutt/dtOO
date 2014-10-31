#include "dtPoint2_surfaceEquidistantPoint.h"
#include <interfaceHeaven/vectorHandling.h>

#include <geometryEngine/dtSurface.h>

namespace dtOO {
	dtPoint2_surfaceEquidistantPoint::dtPoint2_surfaceEquidistantPoint() {
	}

	dtPoint2_surfaceEquidistantPoint::dtPoint2_surfaceEquidistantPoint( dtSurface const * const dtS, int const & nU, int const nV ) {
		_pUV.resize(nU, nV);
		_pXYZ.resize(nU, nV);
		_dMS.resize(nU-1, nV-1);
		_sumM.resize(nV-1, 0);
		_sumS.resize(nU-1, 0);
		
		float minU = dtS->minU();
		float minV = dtS->minV();
		float maxU = dtS->maxU();
		float maxV = dtS->maxV();

		for (int ii=0; ii<nU; ii++) {
			for (int jj=0; jj<nV; jj++) {
        _pUV[ii][jj] 
				= 
				dtPoint2( 
				  minU + ii*(maxU-minU)/(static_cast<float>(nU-1)),
					minV + jj*(maxV-minV)/(static_cast<float>(nV-1)) 
				);        
        _pXYZ[ii][jj] = dtS->getPoint3d(_pUV[ii][jj]);
			}
		}
		
		for (int ii=0; ii<nU-1; ii++) {
			for (int jj=0; jj<nV-1; jj++) {
				dtVector3 MM = _pXYZ[ii+1][jj] - _pXYZ[ii][jj];
				dtVector3 SS = _pXYZ[ii][jj+1] - _pXYZ[ii][jj];
		    _dMS[ii][jj]
				=
				dtVector2( sqrt(MM.squared_length()), sqrt(SS.squared_length()) );
				_sumM[jj] = _sumM[jj] + _dMS[ii][jj].x();
				_sumS[ii] = _sumS[ii] + _dMS[ii][jj].y();
			}
		}

		writeTableToLog();
	}

	dtPoint2_surfaceEquidistantPoint::~dtPoint2_surfaceEquidistantPoint() {
	}
	
	twoDArrayHandling< dtPoint2 > dtPoint2_surfaceEquidistantPoint::result( void ) {
		return _pUV;
	}
	
	void dtPoint2_surfaceEquidistantPoint::writeTableToLog( void ) const {
	  twoDArrayHandling< float > mat(_dMS.size(0), _dMS.size(1));
		for (int ii=0; ii<_dMS.size(0); ii++) {
			for (int jj=0; jj<_dMS.size(1); jj++) {
				mat[ii][jj] = _dMS[ii][jj].x();
			}
		}
	  DTINFOWF(writeTableToLog(), << floatMatrixToString(mat) );	
		
		for (int ii=0; ii<_dMS.size(0); ii++) {
			for (int jj=0; jj<_dMS.size(1); jj++) {
				mat[ii][jj] = _dMS[ii][jj].y();
			}
		}
	  DTINFOWF(writeTableToLog(), << floatMatrixToString(mat) );			
		
	  DTINFOWF(writeTableToLog(), << floatVecToString(_sumM) );		
		DTINFOWF(writeTableToLog(), << floatVecToString(_sumS) );		
	}
}
