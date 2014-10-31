#include "bSplineSurface_bSplineCurveFillConstructOCC.h"

#include <logMe/logMe.h>
#include <geometryEngine/dtOCCSurfaceBase.h>
#include <geometryEngine/dtOCCCurveBase.h>
#include <geometryEngine/dtOCCBSplineCurve.h>
#include <geometryEngine/dtOCCBSplineSurface.h>

#include <Standard_Failure.hxx>
#include <Standard_ErrorHandler.hxx>
#include <Standard_TypeDef.hxx>
#include <Precision.hxx>
#include <Geom_BSplineCurve.hxx>
#include <Geom_BSplineSurface.hxx>
#include <GeomFill_FillingStyle.hxx>
#include <GeomFill_BSplineCurves.hxx>

namespace dtOO {
	bSplineSurface_bSplineCurveFillConstructOCC::bSplineSurface_bSplineCurveFillConstructOCC( 
	  vectorHandling< dtCurve const * > const & cc 
	) {
		dt__THROW_IF(
			cc.size() != 4,
			bSplineSurface_bSplineCurveFillConstructOCC(),
			<< "Only supported with 4 curves." << LOGDEL
			<< DTLOGEVAL(cc.size())
		);
		Handle(Geom_BSplineCurve) C1;
		Handle(Geom_BSplineCurve) C2;
		Handle(Geom_BSplineCurve) C3;
		Handle(Geom_BSplineCurve) C4;
	  
		dtOCCBSplineCurve const * occC;
	  dt__PTRASS( occC, dtOCCBSplineCurve::ConstDownCast(cc[0]) );
		C1 = Handle(Geom_BSplineCurve)::DownCast( occC->OCCRef().getOCC() );
	  dt__PTRASS( occC, dtOCCBSplineCurve::ConstDownCast(cc[1]) );
		C2 = Handle(Geom_BSplineCurve)::DownCast( occC->OCCRef().getOCC() );
	  dt__PTRASS( occC, dtOCCBSplineCurve::ConstDownCast(cc[2]) );
		C3 = Handle(Geom_BSplineCurve)::DownCast( occC->OCCRef().getOCC() );
	  dt__PTRASS( occC, dtOCCBSplineCurve::ConstDownCast(cc[3]) );
		C4 = Handle(Geom_BSplineCurve)::DownCast( occC->OCCRef().getOCC() );		
		
		
		GeomFill_BSplineCurves fill;
		dtOCCSurfaceBase base;	
		dt__TRYOCC(
			fill.Init(
			  C1, C2, C3, C4, 
				GeomFill_FillingStyle::GeomFill_StretchStyle//,
				//1.e-2
			);						
			base.setOCC( fill.Surface() );
		,
			<< "Could not initialize and create filled surface." << LOGDEL
			<< DTLOGEVAL(cc[0]->getPointPercent3d(0.)) << LOGDEL
			<< DTLOGEVAL(cc[0]->getPointPercent3d(1.)) << LOGDEL
			<< DTLOGEVAL(cc[1]->getPointPercent3d(0.)) << LOGDEL
			<< DTLOGEVAL(cc[1]->getPointPercent3d(1.)) << LOGDEL
			<< DTLOGEVAL(cc[2]->getPointPercent3d(0.)) << LOGDEL
			<< DTLOGEVAL(cc[2]->getPointPercent3d(1.)) << LOGDEL
			<< DTLOGEVAL(cc[3]->getPointPercent3d(0.)) << LOGDEL
			<< DTLOGEVAL(cc[3]->getPointPercent3d(1.)) << LOGDEL
		);
		_dtS.reset( new dtOCCBSplineSurface(base) );
	}
	
	bSplineSurface_bSplineCurveFillConstructOCC::~bSplineSurface_bSplineCurveFillConstructOCC() {
	}
	
	dtSurface * bSplineSurface_bSplineCurveFillConstructOCC::result( void ) {
		return _dtS->clone();
	}
}

