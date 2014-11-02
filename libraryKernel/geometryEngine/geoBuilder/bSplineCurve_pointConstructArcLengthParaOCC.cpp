#include "bSplineCurve_pointConstructArcLengthParaOCC.h"
#include "progHelper.h"

#include <logMe/logMe.h>
#include <geometryEngine/dtOCCCurveBase.h>
#include <geometryEngine/dtOCCBSplineCurve.h>
//#include "bSplineCurve_pointConstructOCC.h"
#include <Standard_Failure.hxx>
#include <Standard_ErrorHandler.hxx>
#include <Geom_BSplineCurve.hxx>
#include <Approx_CurvilinearParameter.hxx>
#include <GeomAdaptor_HCurve.hxx>

namespace dtOO {
	bSplineCurve_pointConstructArcLengthParaOCC::bSplineCurve_pointConstructArcLengthParaOCC( 
	  dtCurve const * dtC, float const tol, 
		int const maxOrder, int const segments 
	) {
		const Handle(Geom_BSplineCurve) bsc 
		=
		Handle(Geom_BSplineCurve)::DownCast(
		  dtOCCCurve::ConstDownCast(dtC)->OCCRef().getOCC()
		);
		
		Handle(GeomAdaptor_HCurve) gahc;
		dt__TRYOCC(
			gahc
			=
			new GeomAdaptor_HCurve(bsc);
		, << "");
//		gahc.Load( bsc );
		
		//Standard_Real llR = static_cast<Standard_Real>(length);
		Handle(Geom_BSplineCurve) curve;
		
		dt__TRYOCC(
			Approx_CurvilinearParameter 
			acp(
			  gahc, 
				static_cast<Standard_Real>(tol), 
				GeomAbs_Shape::GeomAbs_C0, 
				static_cast<Standard_Integer>(maxOrder), 
				static_cast<Standard_Integer>(segments) 
			);
			curve = acp.Curve3d();
		, << "");
		
		dtOCCCurveBase base;
		base.setOCC( curve );
		_dtC.reset( new dtOCCBSplineCurve(base) );

		std::vector< float > ul;
		float length = _dtC->l_uPercent(1.);
		ul.push_back(0.);
		ul.push_back( _dtC->l_uPercent(0.)/length );		
		ul.push_back(.25);
		ul.push_back( _dtC->l_uPercent(.25)/length );
		ul.push_back(.5);
		ul.push_back( _dtC->l_uPercent(.5)/length );		
		ul.push_back(.75);
		ul.push_back( _dtC->l_uPercent(.75)/length );		
		ul.push_back(1.00);
		ul.push_back( _dtC->l_uPercent(1.00)/length );		

		std::vector< std::string > header;
		header.push_back("u");
		header.push_back("l(u)/l_ges");
		DTDEBUGWF(
			bSplineCurve_pointConstructArcLengthParaOCC(), 
			<< logMe::floatVecToTable(header, ul)
		);		
	}
	
	bSplineCurve_pointConstructArcLengthParaOCC::~bSplineCurve_pointConstructArcLengthParaOCC() {
	}
	
	dtCurve * bSplineCurve_pointConstructArcLengthParaOCC::result( void ) {
		return _dtC->clone();
	}
}