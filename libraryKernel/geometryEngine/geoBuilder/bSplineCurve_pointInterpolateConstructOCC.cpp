#include "bSplineCurve_pointInterpolateConstructOCC.h"
#include "progHelper.h"

#include <logMe/logMe.h>
#include <geometryEngine/dtOCCCurveBase.h>
#include <geometryEngine/dtOCCBSplineCurve.h>

#include <Precision.hxx>
#include <Geom_BSplineCurve.hxx>
#include <GeomAPI_PointsToBSpline.hxx>
#include <TColgp_HArray1OfPnt.hxx>
#include <TColgp_Array1OfPnt.hxx>
#include <TColStd_HArray1OfReal.hxx>

namespace dtOO {
	bSplineCurve_pointInterpolateConstructOCC::bSplineCurve_pointInterpolateConstructOCC( std::vector < dtPoint3 > const & pp ) {
    //
		// number of points
		//
		int nP = pp.size();
		
		//
		// allocate arraies
		//
		Handle(TColgp_HArray1OfPnt) arr = new TColgp_HArray1OfPnt(1, nP);
		TColgp_Array1OfPnt arr2(1, nP);
		Handle(TColStd_HArray1OfReal) para = new TColStd_HArray1OfReal(1, nP);

		//
		// set arraies
		//
		dt__FORALL(pp, ii,
						DTINFOWF(bSplineCurve_pointInterpolateConstructOCC(),
						<< DTLOGPOI3D(pp[ii]) << LOGDEL
						<< DTLOGEVAL(static_cast<float>(ii)));
		        
		  arr->SetValue( ii+1, gp_Pnt(pp[ii].x(), pp[ii].y(), pp[ii].z()) ); 
			arr2.SetValue( ii+1, gp_Pnt(pp[ii].x(), pp[ii].y(), pp[ii].z()) ); 
		  para->SetValue( ii+1, static_cast<float>(ii) );
//			gp_Pnt(pp[ii].x(), pp[ii].y(), pp[ii].z())
	  );

//		GeomAPI_Interpolate Interp(arr, para, false, Precision::Confusion());
		GeomAPI_PointsToBSpline Interp(arr2);
		
//		if ( !Interp.IsDone() ) {
//			dt__THROW(bSplineCurve_pointInterpolateConstructOCC(), 
//							<< DTLOGEVAL(Interp.IsDone()) );
//		}
//		Interp.Perform();
		Handle(Geom_BSplineCurve) curve = Interp.Curve();
		dtOCCCurveBase base;
		base.setOCC( curve );
		_dtC.reset( new dtOCCBSplineCurve(base) );
	}
	
	bSplineCurve_pointInterpolateConstructOCC::~bSplineCurve_pointInterpolateConstructOCC() {
	}
	
	dtCurve * bSplineCurve_pointInterpolateConstructOCC::result( void ) {
		return _dtC->clone();
	}
}
