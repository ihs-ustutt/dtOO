#include "bSplineCurve_pointConstructOCC.h"
#include "progHelper.h"

#include <logMe/logMe.h>
#include <geometryEngine/dtOCCCurveBase.h>
#include <geometryEngine/dtOCCBSplineCurve.h>

#include <Standard_Failure.hxx>
#include <Geom_BSplineCurve.hxx>
#include <TColgp_Array1OfPnt.hxx>
#include <TColStd_Array1OfReal.hxx>
#include <TColStd_Array1OfInteger.hxx>

namespace dtOO {
	bSplineCurve_pointConstructOCC::bSplineCurve_pointConstructOCC( std::vector< dtPoint3 > const & pp, int const & order ) {
    //
		// number of points
		//
		int const nP = pp.size();
		int const nOnes = nP - (order+1);
		if (nOnes < 0) {
			dt__THROW(
				bSplineCurve_pointConstructOCC(),
				<< "Order too high. Please reduce order of the spline." << LOGDEL
				<< DTLOGEVAL(order) << LOGDEL
				<< DTLOGEVAL(nOnes) 
			);
		}
		
		//
		// allocate arraies
		//
		TColgp_Array1OfPnt poles(1, nP);
		TColStd_Array1OfReal knots(1, nOnes+2);
		TColStd_Array1OfInteger mults(1, nOnes+2);
		mults.Init(1);

		//
		// set poles and multiplicity
		//	
		dt__forAllIndex(pp, ii) {
		  poles.SetValue( ii+1, gp_Pnt(pp[ii].x(), pp[ii].y(), pp[ii].z()) );
	  }
		for (int ii=1;ii<=knots.Length();ii++) {
			knots.SetValue( ii, static_cast<float>(ii-1) );
		}
		mults.SetValue(1, order+1);
		mults.SetValue(nOnes+2, order+1);
		
		//
		// create curve using standard constructor
		//
		Handle(Geom_BSplineCurve) curve 
		= 
		new Geom_BSplineCurve( poles, knots, mults, order );

		dtOCCCurveBase base;
		base.setOCC( curve );
		_dtC.reset( new dtOCCBSplineCurve(base) );
		
//		std::vector< float > ul;
//		float length = _dtC->l_uPercent(1.);
//		ul.push_back(0.);
//		ul.push_back( _dtC->l_uPercent(0.)/length );		
//		ul.push_back(.25);
//		ul.push_back( _dtC->l_uPercent(.25)/length );
//		ul.push_back(.5);
//		ul.push_back( _dtC->l_uPercent(.5)/length );		
//		ul.push_back(.75);
//		ul.push_back( _dtC->l_uPercent(.75)/length );		
//		ul.push_back(1.00);
//		ul.push_back( _dtC->l_uPercent(1.00)/length );		

//		std::vector< std::string > header;
//		header.push_back("u");
//		header.push_back("l(u)/l_ges");
//		DTDEBUGWF(
//			bSplineCurve_pointConstructOCC(), 
//			<< floatVecToTable(header, ul)
//		);
	}
	
	bSplineCurve_pointConstructOCC::~bSplineCurve_pointConstructOCC() {
	}
	
	dtCurve * bSplineCurve_pointConstructOCC::result( void ) {
		return _dtC->clone();
	}
}