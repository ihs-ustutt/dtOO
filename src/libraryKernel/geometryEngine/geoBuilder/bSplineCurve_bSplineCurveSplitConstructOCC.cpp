#include "bSplineCurve_bSplineCurveSplitConstructOCC.h"

#include <progHelper.h>
#include <logMe/logMe.h>

#include <geometryEngine/dtOCCCurveBase.h>
#include <geometryEngine/dtOCCBSplineCurve.h>

#include <Standard_Failure.hxx>
#include <Standard_ErrorHandler.hxx>
#include <Standard_TypeDef.hxx>
#include <Precision.hxx>
#include <Geom_BSplineCurve.hxx>
#include <GeomConvert.hxx>

namespace dtOO {
  bSplineCurve_bSplineCurveSplitConstructOCC
    ::bSplineCurve_bSplineCurveSplitConstructOCC( 
    dtCurve const * const dtC, dtReal const & u0, dtReal const & u1
  ) {
    dt__ptrAss(
      dtOCCBSplineCurve const * const bS,
			dtOCCBSplineCurve::ConstDownCast(dtC)
		);
    
    Handle(Geom_BSplineCurve) geomBSpline 
    = 
    Handle(Geom_BSplineCurve)::DownCast( bS->OCCRef().getOCC() );
    
    dt__throwIf(
      geomBSpline.IsNull(), bSplineCurve_bSplineCurveSplitConstructOCC()
    );

		dtOCCCurveBase base;

    dt__tryOcc(        
      base.setOCC( 
        GeomConvert::SplitBSplineCurve(
          geomBSpline, 
          Standard_Real(u0), Standard_Real(u1), Precision::Confusion()
        )
      );
      ,
      << "Split fails."
    );
		_dtC.reset( new dtOCCBSplineCurve(base) );
  } 
  
	bSplineCurve_bSplineCurveSplitConstructOCC
    ::~bSplineCurve_bSplineCurveSplitConstructOCC() {
	}
	
	dtCurve * bSplineCurve_bSplineCurveSplitConstructOCC::result( void ) {
		return _dtC->clone();
	}
}
