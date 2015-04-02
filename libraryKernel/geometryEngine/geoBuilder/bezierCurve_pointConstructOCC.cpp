#include "bezierCurve_pointConstructOCC.h"
#include "progHelper.h"

#include <logMe/logMe.h>
#include <geometryEngine/dtOCCCurveBase.h>
#include <geometryEngine/dtOCCBezierCurve.h>

#include <Standard_Failure.hxx>
#include <Standard_ErrorHandler.hxx>
#include <Standard_TypeDef.hxx>
#include <Geom_BezierCurve.hxx>
#include <TColgp_HArray1OfPnt.hxx>

namespace dtOO {
	bezierCurve_pointConstructOCC::bezierCurve_pointConstructOCC( std::vector < dtPoint3 > const & pp ) {
    //
		// number of points
		//
		int nP = pp.size();
		
		//
		// allocate arraies
		//
		Handle(TColgp_HArray1OfPnt) arr = new TColgp_HArray1OfPnt(1, nP);
//		Handle(TColStd_HArray1OfReal) para = new TColStd_HArray1OfReal(1, nP);

		//
		// set arraies
		//
		dt__forAllIndex(pp, ii) {
		  arr->SetValue( ii+1, gp_Pnt(pp[ii].x(), pp[ii].y(), pp[ii].z()) ); 
//		  para->SetValue( ii+1, static_cast<float>(ii) );
	  }

//		GeomAPI_Interpolate Interp(arr, para, false, .01);
//		Interp.Perform();
		Handle(Geom_BezierCurve) curve;
		dt__tryOcc(
		  curve = new Geom_BezierCurve( arr->Array1() );
		,
		<< ""
		);
//		dt__info(bezierCurve_pointConstructOCC(),
//						<< dt__eval(curve->Degree()) << std::endl);
		dtOCCCurveBase base;
		base.setOCC( curve );
		_dtC.reset( new dtOCCBezierCurve(base) );
	}
	
	bezierCurve_pointConstructOCC::~bezierCurve_pointConstructOCC() {
	}
	
	dtCurve * bezierCurve_pointConstructOCC::result( void ) {
		return _dtC->clone();
	}
}
