#include "bSplineSurface_bSplineCurveFillConstructOCC.h"
#include "interfaceHeaven/staticPropertiesHandler.h"

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
#include <geometryEngine/GeomFill_BSplineCurvesExtPrecision.h>

namespace dtOO {
	bSplineSurface_bSplineCurveFillConstructOCC
    ::bSplineSurface_bSplineCurveFillConstructOCC( 
	  vectorHandling< dtCurve const * > const & cc 
	) {
		dt__throwIfWithMessage(
			cc.size() != 4,
			bSplineSurface_bSplineCurveFillConstructOCC(),
			<< "Only supported with 4 curves." << std::endl
			<< dt__eval(cc.size())
		);
		Handle(Geom_BSplineCurve) C1;
		Handle(Geom_BSplineCurve) C2;
		Handle(Geom_BSplineCurve) C3;
		Handle(Geom_BSplineCurve) C4;
	  
		dtOCCBSplineCurve const * occC;
	  dt__ptrAss( occC, dtOCCBSplineCurve::ConstDownCast(cc[0]) );
		C1 = Handle(Geom_BSplineCurve)::DownCast( occC->OCCRef().getOCC() );
	  dt__ptrAss( occC, dtOCCBSplineCurve::ConstDownCast(cc[1]) );
		C2 = Handle(Geom_BSplineCurve)::DownCast( occC->OCCRef().getOCC() );
	  dt__ptrAss( occC, dtOCCBSplineCurve::ConstDownCast(cc[2]) );
		C3 = Handle(Geom_BSplineCurve)::DownCast( occC->OCCRef().getOCC() );
	  dt__ptrAss( occC, dtOCCBSplineCurve::ConstDownCast(cc[3]) );
		C4 = Handle(Geom_BSplineCurve)::DownCast( occC->OCCRef().getOCC() );		
		
		
		GeomFill_BSplineCurvesExtPrecision fill;
		dtOCCSurfaceBase base;	
		dt__tryOcc(
			fill.Init(
			  C1, C2, C3, C4, 
				GeomFill_FillingStyle::GeomFill_StretchStyle,
				staticPropertiesHandler::getInstance()->getOptionFloat("xyz_resolution")
			);						
			base.setOCC( fill.Surface() );
		,
			<< "Could not initialize and create filled surface." << std::endl
			<< dt__eval(cc[0]->pointPercent(0.)) << std::endl
			<< dt__eval(cc[0]->pointPercent(1.)) << std::endl
			<< dt__eval(cc[1]->pointPercent(0.)) << std::endl
			<< dt__eval(cc[1]->pointPercent(1.)) << std::endl
			<< dt__eval(cc[2]->pointPercent(0.)) << std::endl
			<< dt__eval(cc[2]->pointPercent(1.)) << std::endl
			<< dt__eval(cc[3]->pointPercent(0.)) << std::endl
			<< dt__eval(cc[3]->pointPercent(1.)) << std::endl
		);
		_dtS.reset( new dtOCCBSplineSurface(base) );
	}

	bSplineSurface_bSplineCurveFillConstructOCC
    ::bSplineSurface_bSplineCurveFillConstructOCC( 
	  dt__pVH(dtCurve) const & cc 
	) {
	  vectorHandling< dtCurve const * > ccV;
		for (
			dt__pVH(dtCurve)::const_iterator it = cc.begin();
			it!=cc.end();
			++it
		) {
			ccV.push_back( &(*it) );
		}
		
		_dtS.reset( bSplineSurface_bSplineCurveFillConstructOCC(ccV).result() );
	}

  bSplineSurface_bSplineCurveFillConstructOCC
    ::bSplineSurface_bSplineCurveFillConstructOCC( 
      dtCurve const * const c0, dtCurve const * const c1, 
      dtCurve const * const c2, dtCurve const * const c3
    ) {
    vectorHandling< dtCurve const * > ccV(4, NULL);
    ccV[0] = c0;
    ccV[1] = c1;
    ccV[2] = c2;
    ccV[3] = c3;
    _dtS.reset( 
      bSplineSurface_bSplineCurveFillConstructOCC(ccV).result() 
    );
  }  
	
	bSplineSurface_bSplineCurveFillConstructOCC
    ::~bSplineSurface_bSplineCurveFillConstructOCC() {
	}
	
	dtSurface * bSplineSurface_bSplineCurveFillConstructOCC::result( void ) {
		return _dtS->clone();
	}
}

