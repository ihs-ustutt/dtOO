#include "geomSurface_geomCurveFillConstructOCC.h"

#include <logMe/logMe.h>
#include <geometryEngine/dtCurve.h>
#include <geometryEngine/dtSurface.h>
#include <geometryEngine/dtOCCCurveBase.h>
#include <geometryEngine/dtOCCSurfaceBase.h>
#include <geometryEngine/dtOCCCurve.h>
#include <geometryEngine/dtOCCBSplineCurve.h>
#include <geometryEngine/dtOCCTrimmedCurve.h>
#include "bSplineCurve_pointConstructOCC.h"
#include "bSplineSurface_bSplineCurveFillConstructOCC.h"
#include <Standard_Failure.hxx>
#include <Standard_ErrorHandler.hxx>
#include <Standard_TypeDef.hxx>
#include <Geom_Curve.hxx>
#include <Geom_TrimmedCurve.hxx>
#include <Geom_Line.hxx>
#include <Precision.hxx>

namespace dtOO {
	geomSurface_geomCurveFillConstructOCC::geomSurface_geomCurveFillConstructOCC( 
	  vectorHandling< dtCurve const * > const & cc 
	) {
		init(cc);
	}
	
	geomSurface_geomCurveFillConstructOCC::geomSurface_geomCurveFillConstructOCC( 
		dtCurve const * c0, dtCurve const * c1, 
		dtCurve const * c2, dtCurve const * c3 
	) {
		vectorHandling< dtCurve const * > cc(4);
		cc[0] = c0;
		cc[1] = c1;
		cc[2] = c2;
		cc[3] = c3;
		
		init(cc);
	}

	void geomSurface_geomCurveFillConstructOCC::init( 
	  vectorHandling< dtCurve const * > const & cc 
	) {
		dt__THROW_IF(
			cc.size() != 4, 
			geomSurface_geomCurveFillConstructOCC(),
			<< "Only supported with 4 curves." << LOGDEL
			<< DTLOGEVAL(cc.size())
		);
		vectorHandling< dtCurve const * > C(4);
		for (int ii=0; ii<4; ii++) {
			dtOCCBSplineCurve const * occBsc = dtOCCBSplineCurve::ConstDownCast(cc[ii]);
			dtOCCTrimmedCurve const * occTc = dtOCCTrimmedCurve::ConstDownCast(cc[ii]);
			if (occBsc) {
				C[ii] = occBsc->clone();
			}
			else if (occTc) {
  			C[ii] = occTc->bSplineCurve();
			}
      else {
				dt__THROW(
				  geomSurface_geomCurveFillConstructOCC(), 
					<< DTLOGEVAL(occBsc) << LOGDEL
					<< DTLOGEVAL(occTc)
				);
			}			
		}

    _dtS.reset( bSplineSurface_bSplineCurveFillConstructOCC(C).result() );
		
		C.destroy();
	}
	
	geomSurface_geomCurveFillConstructOCC::~geomSurface_geomCurveFillConstructOCC() {
	}
	
	dtSurface * geomSurface_geomCurveFillConstructOCC::result( void ) {
		return _dtS->clone();
	}
}