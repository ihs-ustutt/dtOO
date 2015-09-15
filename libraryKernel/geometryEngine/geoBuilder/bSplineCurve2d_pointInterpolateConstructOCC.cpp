#include "bSplineCurve2d_pointInterpolateConstructOCC.h"
#include "progHelper.h"

#include <logMe/logMe.h>
#include <geometryEngine/dtOCCCurve2dBase.h>
#include <geometryEngine/dtOCCBSplineCurve2d.h>

#include <Standard_Failure.hxx>
#include <Standard_ErrorHandler.hxx>
#include <Standard_TypeDef.hxx>
#include <Precision.hxx>
#include <Geom2d_BSplineCurve.hxx>
#include <Geom2dAPI_PointsToBSpline.hxx>
#include <TColgp_HArray1OfPnt2d.hxx>
#include <TColgp_Array1OfPnt2d.hxx>
#include <TColStd_HArray1OfReal.hxx>

namespace dtOO {
	bSplineCurve2d_pointInterpolateConstructOCC
    ::bSplineCurve2d_pointInterpolateConstructOCC( 
    std::vector < dtPoint2 > const & pp 
  ) {
    //
		// number of points
		//
		int nP = pp.size();
		
		//
		// allocate arraies
		//
		Handle(TColgp_HArray1OfPnt2d) arr = new TColgp_HArray1OfPnt2d(1, nP);
		TColgp_Array1OfPnt2d arr2(1, nP);
		Handle(TColStd_HArray1OfReal) para = new TColStd_HArray1OfReal(1, nP);

		//
		// set arraies
		//
		dt__forAllIndex(pp, ii) {		        
		  arr->SetValue( ii+1, gp_Pnt2d(pp[ii].x(), pp[ii].y()) ); 
			arr2.SetValue( ii+1, gp_Pnt2d(pp[ii].x(), pp[ii].y()) ); 
		  para->SetValue( ii+1, static_cast<float>(ii) );
	  }

		dtOCCCurve2dBase base;
    dt__tryOcc(    
      Geom2dAPI_PointsToBSpline Interp(arr2);
      Handle(Geom2d_BSplineCurve) curve = Interp.Curve();
      base.setOCC( curve );
      ,
      << "Interpolation failed."
    );
		_dtC.reset( new dtOCCBSplineCurve2d(base) );
	}
	
	bSplineCurve2d_pointInterpolateConstructOCC
    ::~bSplineCurve2d_pointInterpolateConstructOCC() {
	}
	
	dtCurve2d * bSplineCurve2d_pointInterpolateConstructOCC::result( void ) {
		return _dtC->clone();
	}
}
