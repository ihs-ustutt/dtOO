#include "geomCurve_normalOffsetGeomCurveOCC.h"

#include <logMe/logMe.h>
#include <progHelper.h>
#include <interfaceHeaven/vectorHandling.h>
#include <geometryEngine/dtOCCCurve.h>
#include <geometryEngine/dtOCCCurveBase.h>
#include "bSplineCurve_pointConstructOCC.h"

#include <Precision.hxx>
#include <Standard_Failure.hxx>
#include <Standard_ErrorHandler.hxx>

namespace dtOO {
	geomCurve_normalOffsetGeomCurveOCC::geomCurve_normalOffsetGeomCurveOCC(dtCurve const * const dtC, float const & offset) {
    dt__PTRASS( dtOCCCurve const * occC, dtOCCCurve::ConstDownCast(dtC) );
		
		int numCP = occC->getNControlPoints();
		vectorHandling< dtPoint3 > pp;


		// first
		dtVector3 vv0 = occC->getControlPoint3d(1) - occC->getControlPoint3d(0);
		dtVector3 vv1 = occC->getControlPoint3d(2) - occC->getControlPoint3d(1);
	  dtVector3 nn = dtLinearAlgebra::crossProduct(vv0, vv1);
		dtVector3 nC0 = dtLinearAlgebra::crossProduct(vv0, nn);
		nC0 = dtLinearAlgebra::normalize(nC0);
		dtVector3 nC1 = dtLinearAlgebra::crossProduct(vv1, nn);
		nC1 = dtLinearAlgebra::normalize(nC1);
		dtVector3 nC = .5 * (nC0 + nC1);
		nC = dtLinearAlgebra::normalize(nC);
    pp.push_back(occC->getControlPoint3d(0) + offset*nC0);
			
    // in-between
		for (int ii=1; ii<numCP-1; ii++) {
			vv0 = occC->getControlPoint3d(ii) - occC->getControlPoint3d(ii-1);
			vv1 = occC->getControlPoint3d(ii+1) - occC->getControlPoint3d(ii);
			nn = dtLinearAlgebra::crossProduct(vv0, vv1);
			nC0 = dtLinearAlgebra::crossProduct(vv0, nn);
			nC0 = dtLinearAlgebra::normalize(nC0);
			nC1 = dtLinearAlgebra::crossProduct(vv1, nn);
			nC1 = dtLinearAlgebra::normalize(nC1);
			nC = .5 * (nC0 + nC1);
			nC = dtLinearAlgebra::normalize(nC);
//			pp.push_back(occC->getControlPoint3d(ii) + offset*nC0);
			pp.push_back(occC->getControlPoint3d(ii) + offset*nC);
//			pp.push_back(occC->getControlPoint3d(ii) + offset*nC1);			
		}

		// last
		vv0 = occC->getControlPoint3d(numCP-2) - occC->getControlPoint3d(numCP-3);
		vv1 = occC->getControlPoint3d(numCP-1) - occC->getControlPoint3d(numCP-2);
	  nn = dtLinearAlgebra::crossProduct(vv0, vv1);
		nC0 = dtLinearAlgebra::crossProduct(vv0, nn);
		nC0 = dtLinearAlgebra::normalize(nC0);
		nC1 = dtLinearAlgebra::crossProduct(vv1, nn);
		nC1 = dtLinearAlgebra::normalize(nC1);
		nC = .5 * (nC0 + nC1);
		nC = dtLinearAlgebra::normalize(nC);
    pp.push_back(occC->getControlPoint3d(numCP-1) + offset*nC1);		
		
		_dtC.reset( bSplineCurve_pointConstructOCC(pp, dtC->getOrder()).result() );
	}

	geomCurve_normalOffsetGeomCurveOCC::~geomCurve_normalOffsetGeomCurveOCC() {
	}
	
	dtCurve * geomCurve_normalOffsetGeomCurveOCC::result( void ) {
		return _dtC->clone();
	}	
}
