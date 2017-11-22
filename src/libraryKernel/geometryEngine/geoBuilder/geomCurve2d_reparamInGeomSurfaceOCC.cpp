#include "geomCurve2d_reparamInGeomSurfaceOCC.h"
#include <geometryEngine/dtOCCCurve.h>
#include <geometryEngine/dtOCCCurve2d.h>
#include <geometryEngine/dtOCCSurfaceBase.h>
#include <geometryEngine/dtOCCSurface.h>
#include <geometryEngine/dtOCCCurveBase.h>
#include <geometryEngine/dtOCCCurve2dBase.h>
#include <geometryEngine/dtOCCBSplineCurve.h>
#include <geometryEngine/dtOCCBezierCurve.h>
#include <geometryEngine/dtOCCBSplineCurve2d.h>
#include "geomCurve2d_baseConstructOCC.h"

#include <Geom_Surface.hxx>
#include <Geom_Curve.hxx>
#include <Geom2d_Curve.hxx>
#include <GeomProjLib.hxx>

namespace dtOO {
	geomCurve2d_reparamInGeomSurfaceOCC::geomCurve2d_reparamInGeomSurfaceOCC() {
		
	}

	geomCurve2d_reparamInGeomSurfaceOCC::geomCurve2d_reparamInGeomSurfaceOCC(
      dtCurve const * const dtC, 
      dtSurface const * const dtS
	) {
		dt__ptrAss( dtOCCSurface const * const ss, dtOCCSurface::ConstDownCast(dtS) );
		dt__ptrAss( dtOCCCurve const * const cc, dtOCCCurve::ConstDownCast(dtC) );
		
		Geom_Surface const * sP;
		dt__mustCast(ss->OCCRef().getOCC().Access(), Geom_Surface const, sP);
		Geom_Curve const * cP;
		dt__mustCast(cc->OCCRef().getOCC().Access(), Geom_Curve const, cP);
		Handle(Geom_Curve) cH = cP;
		Handle(Geom_Surface) sH = sP;
		
		Handle(Geom2d_Curve) pC = GeomProjLib::Curve2d(cH, sH);
		
		if (pC.IsNull()) {
			dt__throw(
				geomCurve2d_reparamInGeomSurfaceOCC(),
				<< "Projection of curve to surface fails."
			);
		}

		dtOCCCurve2dBase base;
		base.setOCC(pC);
    _dtC2d.reset( geomCurve2d_baseConstructOCC(base).result() );
	}

	geomCurve2d_reparamInGeomSurfaceOCC::~geomCurve2d_reparamInGeomSurfaceOCC() {
		
	}
	
	dtCurve2d * geomCurve2d_reparamInGeomSurfaceOCC::result( void ) {
		return _dtC2d->clone();		
	}
}
