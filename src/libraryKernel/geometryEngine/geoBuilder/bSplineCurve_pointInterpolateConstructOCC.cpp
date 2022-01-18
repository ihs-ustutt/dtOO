#include "bSplineCurve_pointInterpolateConstructOCC.h"
#include "progHelper.h"

#include <logMe/logMe.h>
#include <geometryEngine/dtOCCCurveBase.h>
#include <geometryEngine/dtOCCBSplineCurve.h>

#include <Standard_Failure.hxx>
#include <Standard_ErrorHandler.hxx>
#include <Standard_TypeDef.hxx>
#include <Precision.hxx>
#include <Geom_BSplineCurve.hxx>
#include <GeomAPI_PointsToBSpline.hxx>
#include <TColgp_HArray1OfPnt.hxx>
#include <TColgp_Array1OfPnt.hxx>
#include <TColStd_HArray1OfReal.hxx>
#include <GeomAbs_Shape.hxx>

namespace dtOO {
	bSplineCurve_pointInterpolateConstructOCC
    ::bSplineCurve_pointInterpolateConstructOCC( 
    std::vector < dtPoint3 > const & pp 
  ) {
    _dtC.reset( 
      bSplineCurve_pointInterpolateConstructOCC(pp, 3, 8, 4).result() 
    );
	}
	
  bSplineCurve_pointInterpolateConstructOCC
    ::bSplineCurve_pointInterpolateConstructOCC( 
    std::vector < dtPoint3 > const & pp, dtInt const & degMin, dtInt const & degMax
  ) {
    _dtC.reset( 
      bSplineCurve_pointInterpolateConstructOCC(pp, degMin, degMax, 4).result() 
    );
    
  }
  
  bSplineCurve_pointInterpolateConstructOCC
    ::bSplineCurve_pointInterpolateConstructOCC( 
    std::vector < dtPoint3 > const & pp, dtInt const & degMin, dtInt const & degMax,
    dtInt const & cont
  ) {
    //
		// number of points
		//
		int nP = pp.size();
		
		//
		// allocate arrays
		//
		TColgp_Array1OfPnt arr2(1, nP);
		Handle(TColStd_HArray1OfReal) para = new TColStd_HArray1OfReal(1, nP);

		//
		// set arrays
		//
		dt__forAllIndex(pp, ii) {		        
			arr2.SetValue( ii+1, gp_Pnt(pp[ii].x(), pp[ii].y(), pp[ii].z()) ); 
		  para->SetValue( ii+1, static_cast<dtReal>(ii) );
	  }
		dtOCCCurveBase base;
    dt__tryOcc(    
      GeomAPI_PointsToBSpline Interp(arr2, degMin, degMax, cont);
      Handle(Geom_BSplineCurve) curve = Interp.Curve();
      base.setOCC( curve );
      ,
      << "Interpolation failed."
    );
		_dtC.reset( new dtOCCBSplineCurve(base) );    
  } 
  
	bSplineCurve_pointInterpolateConstructOCC
    ::~bSplineCurve_pointInterpolateConstructOCC() {
	}
	
	dtCurve * bSplineCurve_pointInterpolateConstructOCC::result( void ) {
		return _dtC->clone();
	}
}
