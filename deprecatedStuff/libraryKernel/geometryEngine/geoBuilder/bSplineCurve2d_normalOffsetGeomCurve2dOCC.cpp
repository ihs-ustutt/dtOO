#include "bSplineCurve2d_normalOffsetGeomCurve2dOCC.h"

#include <logMe/logMe.h>
#include <progHelper.h>
#include <interfaceHeaven/vectorHandling.h>
#include <geometryEngine/dtCurve2d.h>
#include "bSplineCurve2d_pointConstructOCC.h"

namespace dtOO {
	bSplineCurve2d_normalOffsetGeomCurve2dOCC
	  ::bSplineCurve2d_normalOffsetGeomCurve2dOCC(
	    dtCurve2d const * const dtC2d, std::vector< float > const & offset
	) {
//    dt__THROW_IF(
//			dtC2d->nControlPoints()<3, 
//			bSplineCurve2d_normalOffsetGeomCurve2dOCC()
//		);

		int nPoints = offset.size();
		vectorHandling< dtPoint2 > pp;


//		// first
//		dtVector2 vv0 = dtC2d->controlPoint(1) - dtC2d->controlPoint(0);
//		dtVector2 vv1 = dtC2d->controlPoint(2) - dtC2d->controlPoint(1);
//		dtVector2 nC0 = dtLinearAlgebra::unitNormal(vv0);
//		dtVector2 nC1 = dtLinearAlgebra::unitNormal(vv1);
//		dtVector2 nC = nC0;//.5 * (nC0 + nC1);
//    pp.push_back(dtC2d->controlPoint(0) + offset.front()*nC0);
			
    // in-between
//		float dist = static_cast<float>(1-nPoints);
		for (int ii=0; ii<nPoints; ii++) {
//			vv0 = dtC2d->controlPoint(ii) - dtC2d->controlPoint(ii-1);
//			vv1 = dtC2d->controlPoint(ii+1) - dtC2d->controlPoint(ii);
//			nC0 = dtLinearAlgebra::unitNormal(vv0);
//			nC1 = dtLinearAlgebra::unitNormal(vv1);
			float per = static_cast<float>(ii)/static_cast<float>(nPoints-1);
			
			dtVector2 nC 
		  = 
			dtLinearAlgebra::unitNormal( dtC2d->firstDer(dtC2d->u_uPercent(per)) );
			pp.push_back(
			  dtC2d->point( dtC2d->u_uPercent(per) ) + offset[ii]*nC
			);
		}

//		// last
//		vv0 = dtC2d->controlPoint(numCP-2) - dtC2d->controlPoint(numCP-3);
//		vv1 = dtC2d->controlPoint(numCP-1) - dtC2d->controlPoint(numCP-2);
//		nC0 = dtLinearAlgebra::unitNormal(vv0);
//		nC1 = dtLinearAlgebra::unitNormal(vv1);
//		nC = nC1;//.5 * (nC0 + nC1);
//    pp.push_back(dtC2d->controlPoint(numCP-1) + offset.back()*nC1);		
		
		_dtC2d.reset( bSplineCurve2d_pointConstructOCC(pp, dtC2d->order()).result() );
	}
	
	bSplineCurve2d_normalOffsetGeomCurve2dOCC
	  ::bSplineCurve2d_normalOffsetGeomCurve2dOCC(
		  dtCurve2d const * const dtC, float const & offset, int const & nPoints
	) {
		_dtC2d.reset(
		  bSplineCurve2d_normalOffsetGeomCurve2dOCC(
		    dtC, std::vector< float >(nPoints, offset)
		  ).result()
		);
	} 	

	bSplineCurve2d_normalOffsetGeomCurve2dOCC::~bSplineCurve2d_normalOffsetGeomCurve2dOCC() {
	}
	
	dtCurve2d * bSplineCurve2d_normalOffsetGeomCurve2dOCC::result( void ) {
		return _dtC2d->clone();
	}	
}
