#include "geomCurve_baseConstructOCC.h"
#include <logMe/dtMacros.h>

#include <logMe/logMe.h>
#include <geometryEngine/dtOCCCurveBase.h>
#include <geometryEngine/dtCurve.h>
#include <geometryEngine/dtOCCCurve.h>
#include <geometryEngine/dtOCCBSplineCurve.h>
#include <geometryEngine/dtOCCBezierCurve.h>
#include <geometryEngine/dtOCCTrimmedCurve.h>
#include <geometryEngine/dtOCCLine.h>

#include <Precision.hxx>
#include <Standard_Failure.hxx>
#include <Standard_ErrorHandler.hxx>
#include <Standard_TypeDef.hxx>
#include <Geom_Curve.hxx>
#include <Geom_BSplineCurve.hxx>
#include <Geom_BezierCurve.hxx>
#include <Geom_TrimmedCurve.hxx>
#include <Geom_Line.hxx>

namespace dtOO {
	geomCurve_baseConstructOCC::geomCurve_baseConstructOCC( dtOCCCurveBase const & base ) {
		Handle(Geom_Curve) occC = base.getOCC();
		if ( !(Handle(Geom_BSplineCurve)::DownCast(occC).IsNull()) ) {
		  _dtC.reset( new dtOCCBSplineCurve(base) );			
		}
		else if ( !(Handle(Geom_BezierCurve)::DownCast(occC).IsNull()) ) {
		  _dtC.reset( new dtOCCBezierCurve(base) );			
		}
//		else if ( !(Handle(Geom_TrimmedCurve)::DownCast(occC).IsNull()) ) {
//			dt__ptrAss(
//				dtOCCTrimmedCurve2d const * const tC2d, 
//				dtOCCTrimmedCurve2d::ConstDownCast(dtC2d)
//			);
//		  _dtC.reset( new dtOCCTrimmedCurve(base, tC2d->getU0(), tC2d->getU1() ) );
//		}		
		else if ( !(Handle(Geom_Line)::DownCast(occC).IsNull()) ) {
		  _dtC.reset( new dtOCCLine(base) );			
		}
		else {
			dt__throw(
				geomCurve_baseConstructOCC(), 
				<< dt__eval(occC->DynamicType()->Name()) << std::endl
				<< "Unsupported OCC type."
			);			
		}
	}
	
	geomCurve_baseConstructOCC::~geomCurve_baseConstructOCC() {
	}
	
	dtCurve * geomCurve_baseConstructOCC::result( void ) {
		return _dtC->clone();
	}
}
