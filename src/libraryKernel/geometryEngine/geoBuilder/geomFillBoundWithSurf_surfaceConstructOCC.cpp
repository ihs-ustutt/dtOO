#include "geomFillBoundWithSurf_surfaceConstructOCC.h"

#include <logMe/logMe.h>
#include <geometryEngine/dtCurve.h>
#include <geometryEngine/dtSurface.h>
#include <geometryEngine/dtOCCSurface.h>
#include <geometryEngine/dtOCCSurfaceBase.h>
#include <geometryEngine/dtCurve2d.h>
#include <geometryEngine/dtOCCCurve2d.h>
#include <geometryEngine/dtOCCCurve2dBase.h>
#include <geometryEngine/dtOCCGeomFillBoundary.h>
#include <geometryEngine/dtOCCBSplineCurve.h>
#include <geometryEngine/dtOCCCurveBase.h>
#include <geometryEngine/dtOCCGeomFillBoundaryBase.h>
#include <geometryEngine/dtOCCGeomFillBoundWithSurf.h>
#include <geometryEngine/geoBuilder/bSplineCurve_approxGeomCurve2dInGeomSurface.h>

#include <Standard_Failure.hxx>
#include <Standard_ErrorHandler.hxx>
#include <Standard_TypeDef.hxx>
#include <Geom_Surface.hxx>
#include <Geom2d_Curve.hxx>
#include <Geom_BSplineCurve.hxx>
#include <GeomFill_BoundWithSurf.hxx>
#include <Adaptor2d_HCurve2d.hxx>
#include <Adaptor3d_HSurface.hxx>
#include <Adaptor3d_CurveOnSurface.hxx>
#include <GeomAdaptor_HSurface.hxx>
#include <Geom2dAdaptor_HCurve.hxx>

namespace dtOO {
	geomFillBoundWithSurf_surfaceConstructOCC
    ::geomFillBoundWithSurf_surfaceConstructOCC( 
      dtSurface const * const dtS, dtCurve2d const * const dtC2d
  ) {
    dt__ptrAss(
		  dtOCCSurface const * occS, 
			dtOCCSurface::ConstDownCast(dtS)
		);
    Handle(Geom_Surface) mySurface
    = 
    Handle(Geom_Surface)::DownCast( occS->OCCRef().getOCC()->Copy() );
    dt__ptrAss(
		  dtOCCCurve2d const * occC2d, 
			dtOCCCurve2d::ConstDownCast(dtC2d)
		);
    Handle(Geom2d_Curve) myParamCurve
    = 
    Handle(Geom2d_Curve)::DownCast( occC2d->OCCRef().getOCC()->Copy() );
    
    Handle(GeomAdaptor_HSurface) Surface 
    = 
    new GeomAdaptor_HSurface(mySurface);
    Handle(Geom2dAdaptor_HCurve) ParamCurve 
    = 
    new Geom2dAdaptor_HCurve(myParamCurve);
    Adaptor3d_CurveOnSurface CurveOnSurf(ParamCurve,Surface);
    
    Handle(GeomFill_BoundWithSurf) myBoundary 
    = 
    new GeomFill_BoundWithSurf(
      CurveOnSurf, Precision::Confusion(), Precision::Angular()
    );
    
    ptrHandling<dtCurve> bsC( 
      bSplineCurve_approxGeomCurve2dInGeomSurface(dtC2d, dtS).result()
    );
    Handle(Geom_Curve) bsCurve
    = 
    Handle(Geom_Curve)::DownCast(
      dtOCCBSplineCurve::DownCast(bsC.get())->OCCRef().getOCC()->Copy()
    );
		dtOCCCurveBase baseC;
		baseC.setOCC( bsCurve );
    dtOCCGeomFillBoundaryBase baseB;
    baseB.setOCC(myBoundary);
		_dtC.reset( new dtOCCGeomFillBoundWithSurf(baseC, baseB) );
	}
	
	geomFillBoundWithSurf_surfaceConstructOCC
    ::~geomFillBoundWithSurf_surfaceConstructOCC() {
	}
	
	dtCurve * geomFillBoundWithSurf_surfaceConstructOCC::result( void ) {
		return _dtC->clone();
	}
}