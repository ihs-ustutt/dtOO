#include "bSplineCurve_curveConnectConstructOCC.h"
#include <logMe/dtMacros.h>

#include <logMe/logMe.h>
#include <geometryEngine/dtOCCCurveBase.h>
#include <geometryEngine/dtOCCBSplineCurve.h>

#include <Precision.hxx>
#include <Geom_BoundedCurve.hxx>
#include <Geom_BSplineCurve.hxx>
#include <GeomConvert_CompCurveToBSplineCurve.hxx>
#include <Standard_Failure.hxx>
#include <Standard_ErrorHandler.hxx>

namespace dtOO {   
	bSplineCurve_curveConnectConstructOCC::bSplineCurve_curveConnectConstructOCC( 
	  vectorHandling< dtCurve const * > const & cc, 
    dtReal const & tol, int const & param, int const & minM
	) {
		GeomConvert_CompCurveToBSplineCurve conv(
      static_cast< Convert_ParameterisationType >(param)
    );
		dt__forAllIndex(cc, ii) {
			dt__ptrAss( dtOCCCurve const * occC, dtOCCCurve::ConstDownCast(cc[ii]) );
			Handle(Geom_BoundedCurve) boundedC
			=
			Handle(Geom_BoundedCurve)::DownCast( occC->OCCRef().getOCC() );
      dt__forFromToIndex(0,8,tt) {
        Standard_Boolean check;        
        dt__tryOcc(
          check 
          =
          conv.Add(
            boundedC, 
            pow(10.,dtReal(tt)) * tol, 
            Standard_False, Standard_True, minM
          );
          ,
          << ""        
        );
        if (check) break;          
        dt__warning(
          bSplineCurve_curveConnectConstructOCC(), 
          << "Cannot connect curves with tolerance " << pow(10.,dtReal(tt)) * tol
          << "\nMultiply tolerance by 10."
        );
      }        
    }
		dtOCCCurveBase base;
		base.setOCC( conv.BSplineCurve() );
		_dtC.reset( new dtOCCBSplineCurve(base) );		
	}
	
	bSplineCurve_curveConnectConstructOCC::bSplineCurve_curveConnectConstructOCC( 
	  vectorHandling< dtCurve const * > const & cc
	) {
    _dtC.reset(
      bSplineCurve_curveConnectConstructOCC(
        cc, Precision::Confusion(), 0, 0
    ).result());
  } 
  
  bSplineCurve_curveConnectConstructOCC::bSplineCurve_curveConnectConstructOCC( 
    dtCurve const * const c0,  dtCurve const * const c1 
  ) {
    vectorHandling< dtCurve const * > cc;
    cc.push_back(c0);
    cc.push_back(c1);
    _dtC.reset( bSplineCurve_curveConnectConstructOCC(cc).result() );
  }
  
	bSplineCurve_curveConnectConstructOCC
    ::~bSplineCurve_curveConnectConstructOCC() {
    
	}
	
	dtCurve * bSplineCurve_curveConnectConstructOCC::result( void ) {
		return _dtC->clone();
	}
}
