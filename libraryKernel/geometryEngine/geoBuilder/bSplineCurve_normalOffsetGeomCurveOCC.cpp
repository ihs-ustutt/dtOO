#include "bSplineCurve_normalOffsetGeomCurveOCC.h"

#include <logMe/logMe.h>
#include <progHelper.h>
#include <interfaceHeaven/vectorHandling.h>
#include "bSplineCurve_pointConstructOCC.h"
#include "geometryEngine/dtCurve.h"

namespace dtOO {
	bSplineCurve_normalOffsetGeomCurveOCC::bSplineCurve_normalOffsetGeomCurveOCC(dtCurve const * const dtC, float const & offset) {
    dt__throwIf(dtC->nControlPoints()<3, bSplineCurve_normalOffsetGeomCurveOCC());
		
		int numCP = dtC->nControlPoints();
		vectorHandling< dtPoint3 > pp;


		// first
		dtVector3 vv0 = dtC->controlPoint(1) - dtC->controlPoint(0);
		dtVector3 vv1 = dtC->controlPoint(2) - dtC->controlPoint(1);
	  dtVector3 nn = dtLinearAlgebra::crossProduct(vv0, vv1);
		dtVector3 nC0 = dtLinearAlgebra::crossProduct(vv0, nn);
		nC0 = dtLinearAlgebra::normalize(nC0);
		dtVector3 nC1 = dtLinearAlgebra::crossProduct(vv1, nn);
		nC1 = dtLinearAlgebra::normalize(nC1);
		dtVector3 nC = .5 * (nC0 + nC1);
		nC = dtLinearAlgebra::normalize(nC);
    pp.push_back(dtC->controlPoint(0) + offset*nC0);
			
    // in-between
		for (int ii=1; ii<numCP-1; ii++) {
			vv0 = dtC->controlPoint(ii) - dtC->controlPoint(ii-1);
			vv1 = dtC->controlPoint(ii+1) - dtC->controlPoint(ii);
			nn = dtLinearAlgebra::crossProduct(vv0, vv1);
			nC0 = dtLinearAlgebra::crossProduct(vv0, nn);
			nC0 = dtLinearAlgebra::normalize(nC0);
			nC1 = dtLinearAlgebra::crossProduct(vv1, nn);
			nC1 = dtLinearAlgebra::normalize(nC1);
			nC = .5 * (nC0 + nC1);
			nC = dtLinearAlgebra::normalize(nC);
//			pp.push_back(dtC->controlPoint(ii) + offset*nC0);
			pp.push_back(dtC->controlPoint(ii) + offset*nC);
//			pp.push_back(dtC->controlPoint(ii) + offset*nC1);			
		}

		// last
		vv0 = dtC->controlPoint(numCP-2) - dtC->controlPoint(numCP-3);
		vv1 = dtC->controlPoint(numCP-1) - dtC->controlPoint(numCP-2);
	  nn = dtLinearAlgebra::crossProduct(vv0, vv1);
		nC0 = dtLinearAlgebra::crossProduct(vv0, nn);
		nC0 = dtLinearAlgebra::normalize(nC0);
		nC1 = dtLinearAlgebra::crossProduct(vv1, nn);
		nC1 = dtLinearAlgebra::normalize(nC1);
		nC = .5 * (nC0 + nC1);
		nC = dtLinearAlgebra::normalize(nC);
    pp.push_back(dtC->controlPoint(numCP-1) + offset*nC1);		
		
		_dtC.reset( bSplineCurve_pointConstructOCC(pp, dtC->order()).result() );
	}

	bSplineCurve_normalOffsetGeomCurveOCC::~bSplineCurve_normalOffsetGeomCurveOCC() {
	}
	
	dtCurve * bSplineCurve_normalOffsetGeomCurveOCC::result( void ) {
		return _dtC->clone();
	}	
}
