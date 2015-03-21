#include "geomCurve_projectOnGeomSurfaceOCC.h"
#include "geometryEngine/dtOCCCurve.h"
#include "geometryEngine/dtOCCSurface.h"
#include <geometryEngine/dtOCCCurveBase.h>
#include <geometryEngine/dtOCCBSplineCurve.h>
#include <geometryEngine/dtOCCBezierCurve.h>

#include <Geom_Surface.hxx>
#include <Geom_Curve.hxx>
#include <GeomProjLib.hxx>

namespace dtOO {
	geomCurve_projectOnGeomSurfaceOCC::geomCurve_projectOnGeomSurfaceOCC() {
		
	}

	geomCurve_projectOnGeomSurfaceOCC::geomCurve_projectOnGeomSurfaceOCC(
      dtCurve const * const dtC, 
      dtSurface const * const dtS
	) {
		dt__ptrAss( dtOCCSurface const * const ss, dtOCCSurface::ConstDownCast(dtS) );
		dt__ptrAss( dtOCCCurve const * const cc, dtOCCCurve::ConstDownCast(dtC) );
		
		Geom_Surface const * sP;
		dt__mustDownCast(ss->OCCRef().getOCC().Access(), Geom_Surface const, sP);
		Geom_Curve const * cP;
		dt__mustDownCast(cc->OCCRef().getOCC().Access(), Geom_Curve const, cP);
		Handle(Geom_Curve) cH = cP;
		Handle(Geom_Surface) sH = sP;
		
		Handle(Geom_Curve) pC = GeomProjLib::Project(cH, sH);
		
		if (pC.IsNull()) {
			dt__THROW(
				geomCurve_projectOnGeomSurfaceOCC(),
				<< "Projection of curve to surface fails."
			);
		}
		
		dtOCCCurveBase base;
		base.setOCC(pC);
		if ( dtOCCBezierCurve::ConstDownCast(dtC) ) {
			_dtC.reset( new dtOCCBezierCurve(base) );		
		}
		else if( dtOCCBSplineCurve::ConstDownCast(dtC) ) {
		  _dtC.reset( new dtOCCBSplineCurve(base) );		
		}
		else {
			dt__THROW(
			  geomCurve_projectOnGeomSurfaceOCC(),
				<< DTLOGEVAL(dtOCCBezierCurve::ConstDownCast(dtC)) << LOGDEL
				<< DTLOGEVAL(dtOCCBSplineCurve::ConstDownCast(dtC))
			);
		}
	}

	geomCurve_projectOnGeomSurfaceOCC::~geomCurve_projectOnGeomSurfaceOCC() {
		
	}
	
	dtCurve * geomCurve_projectOnGeomSurfaceOCC::result( void ) {
		return _dtC->clone();		
	}
}
