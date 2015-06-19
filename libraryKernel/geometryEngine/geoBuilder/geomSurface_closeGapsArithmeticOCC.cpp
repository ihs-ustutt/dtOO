#include "geomSurface_closeGapsArithmeticOCC.h"

#include <geometryEngine/dtSurface.h>
#include <geometryEngine/dtCurve.h>
#include "geomCurve_closeGapsArithmeticOCC.h"
#include "bSplineSurface_skinConstructOCC.h"

namespace dtOO {
	geomSurface_closeGapsArithmeticOCC::geomSurface_closeGapsArithmeticOCC() {
		
	}

	geomSurface_closeGapsArithmeticOCC::geomSurface_closeGapsArithmeticOCC(
    dt__pVH(dtSurface) const & dtS, int const & dir, int const & nSegments
	) {
    //
    // small check
    //
    dt__forAllConstIter(dt__pVH(dtSurface), dtS, it ) {
      dtSurface const & theSurface = *it;
      dt__throwIf(
        theSurface.nControlPoints(dir)!=dtS[0].nControlPoints(dir),
        geomSurface_closeGapsArithmeticOCC()
      );
    }
    
    //
    // pick constant parameter curves of surfaces and close curves
    //
    dt__vH(dt__pVH(dtCurve)) cc(nSegments);    
		for ( int ii=0; ii<nSegments; ii++) {
      dt__forAllConstIter(dt__pVH(dtSurface), dtS, it ) {
        dtSurface const & theSurface = *it;
        float at = ii * 1./(nSegments-1);
        cc[ii].push_back( theSurface.segmentConstPercent(dir, at) );
      }
      cc[ii] = geomCurve_closeGapsArithmeticOCC(cc[ii]).result();
		}

    //
    // create new bSplineSurface of all segment curves
    //
    dt__vH(dtCurve const *) ccT(nSegments);
    dt__forFromToIndex(0, cc[0].size(), jj) {
      dt__forAllIndex(cc, ii) {
        dtCurve const & theCurve = cc[ii][jj];
        ccT[ii] = &theCurve;
      }
      _dtS.push_back( bSplineSurface_skinConstructOCC(ccT).result() );
    }
	}

	geomSurface_closeGapsArithmeticOCC::~geomSurface_closeGapsArithmeticOCC() {
		
	}
	
	dt__pVH(dtSurface) geomSurface_closeGapsArithmeticOCC::result( void ) {
		return _dtS;		
	}
}
