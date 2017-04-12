#include "geomCurve2d_closeGapsArithmeticOCC.h"
#include <geometryEngine/dtCurve2d.h>

namespace dtOO {
	geomCurve2d_closeGapsArithmeticOCC::geomCurve2d_closeGapsArithmeticOCC() {
		
	}

	geomCurve2d_closeGapsArithmeticOCC::geomCurve2d_closeGapsArithmeticOCC(
      dt__pVH(dtCurve2d) const & dtC2d
	) {
		
		typedef std::vector< std::pair< dtVector2, dtVector2 > > pPair;
		pPair pp;
		dt__forAllConstIter(dt__pVH(dtCurve2d), dtC2d, it) {
			dtCurve2d const & theCurve = *it;
			pp.push_back(
				std::pair< dtVector2, dtVector2 >(
					dtLinearAlgebra::toDtVector2(
			      theCurve.controlPoint(0)
			    ), 
					dtLinearAlgebra::toDtVector2(
			      theCurve.controlPoint(theCurve.nControlPoints()-1)
			    ) 
				)
			);
		}
		pp.push_back(pp[0]);
		
    dt__forInnerIndex(pp, ii) {		
			pp[ii].first = 0.5 * (pp[ii].first + pp[ii-1].second);
			pp[ii-1].second = pp[ii].first;

			pp[ii].second = 0.5 * (pp[ii+1].first + pp[ii].second);
			pp[ii+1].first = pp[ii].second;
		}
		
		pp.front().first = pp.back().first;
		pp.pop_back();
		
		dt__forAllConstIter(dt__pVH(dtCurve2d), dtC2d, it) {
			int ii = it - dtC2d.begin();
			dtCurve2d const & theCurve = *it;
			dtCurve2d * theClone = theCurve.clone();
			theClone->setControlPoint(
			  0, dtLinearAlgebra::toDtPoint2(pp[ii].first) 
			);
			theClone->setControlPoint( 
			  theClone->nControlPoints()-1, 
				dtLinearAlgebra::toDtPoint2(pp[ii].second)
			);
			_dtC2d.push_back(theClone);
		}
	}

	geomCurve2d_closeGapsArithmeticOCC::~geomCurve2d_closeGapsArithmeticOCC() {
		
	}
	
	dt__pVH(dtCurve2d) geomCurve2d_closeGapsArithmeticOCC::result( void ) {
		return _dtC2d;		
	}
}
