#include "bSplineCurve_curveConnectConstructOCC.h"
#include <progHelper.h>

#include <logMe/logMe.h>
#include <geometryEngine/dtOCCCurveBase.h>
#include <geometryEngine/dtOCCBSplineCurve.h>

#include <Precision.hxx>
#include <Geom_BoundedCurve.hxx>
#include <Geom_BSplineCurve.hxx>
#include <GeomConvert_CompCurveToBSplineCurve.hxx>

namespace dtOO {
	bSplineCurve_curveConnectConstructOCC::bSplineCurve_curveConnectConstructOCC( 
	  vectorHandling< dtCurve const * > const & cc 
	) {
		GeomConvert_CompCurveToBSplineCurve conv;
		dt__forAllIndex(cc, ii) {
			dt__ptrAss( dtOCCCurve const * occC, dtOCCCurve::ConstDownCast(cc[ii]) );
			Handle(Geom_BoundedCurve) boundedC
			=
			Handle(Geom_BoundedCurve)::DownCast( occC->OCCRef().getOCC() );
      conv.Add(boundedC, Precision::Confusion());//Precision::Confusion());			
    }
		dtOCCCurveBase base;
		base.setOCC( conv.BSplineCurve() );
		_dtC.reset( new dtOCCBSplineCurve(base) );		
	}
	
	bSplineCurve_curveConnectConstructOCC::~bSplineCurve_curveConnectConstructOCC() {
	}
	
	dtCurve * bSplineCurve_curveConnectConstructOCC::result( void ) {
		return _dtC->clone();
	}
}
