#include "bSplineCurve_poleWeightKnotMultOrderConstructOCC.h"
#include "progHelper.h"

#include <logMe/logMe.h>
#include <geometryEngine/dtOCCCurveBase.h>
#include <geometryEngine/dtOCCBSplineCurve.h>

#include <Standard_Failure.hxx>
#include <Standard_ErrorHandler.hxx>
#include <Geom_BSplineCurve.hxx>
#include <TColgp_Array1OfPnt.hxx>
#include <TColStd_Array1OfReal.hxx>
#include <TColStd_Array1OfInteger.hxx>

namespace dtOO {
	bSplineCurve_poleWeightKnotMultOrderConstructOCC
    ::bSplineCurve_poleWeightKnotMultOrderConstructOCC( 
    std::vector< dtPoint3 > const & pole, 
    std::vector< float > const & weight, 
    std::vector< float > const & knot, 
    std::vector< int > const & mult, 
    int const & order 
  ) {
    //
    // convert arrays
    //
    TColgp_Array1OfPnt poles(1, pole.size());
		dt__forAllIndex(pole, ii) {
		  poles.SetValue( ii+1, gp_Pnt(pole[ii].x(), pole[ii].y(), pole[ii].z()) );
	  }
    TColStd_Array1OfReal weights(1, weight.size());
		dt__forAllIndex(weight, ii) weights.SetValue( ii+1, weight[ii] );    
    TColStd_Array1OfReal knots(1, knot.size());
		dt__forAllIndex(knot, ii) knots.SetValue( ii+1, knot[ii] );
    TColStd_Array1OfInteger mults(1, mult.size());
		dt__forAllIndex(mult, ii) mults.SetValue( ii+1, mult[ii] );    

		//
		// create curve
		//
		Handle(Geom_BSplineCurve) curve;
    
    dt__tryOcc(      
      curve = new Geom_BSplineCurve( poles, weights, knots, mults, order );
      ,
      << "Cannot create Geom_BSplineCurve."
    );
		dtOCCCurveBase base;
		base.setOCC( curve );
		_dtC.reset( new dtOCCBSplineCurve(base) );
	}
	
	bSplineCurve_poleWeightKnotMultOrderConstructOCC
    ::~bSplineCurve_poleWeightKnotMultOrderConstructOCC() {
    
	}
	
	dtCurve * bSplineCurve_poleWeightKnotMultOrderConstructOCC::result( void ) {
		return _dtC->clone();
	}
}