/*---------------------------------------------------------------------------*\
  dtOO < design tool Object-Oriented >
    
    Copyright (C) 2024 A. Tismer.
-------------------------------------------------------------------------------
License
    This file is part of dtOO.

    dtOO is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the LICENSE.txt file in the
    dtOO root directory for more details.

    You should have received a copy of the License along with dtOO.

\*---------------------------------------------------------------------------*/

#include "geomCurve_projectOnGeomSurfaceOCC.h"
#include <geometryEngine/dtOCCCurveBase.h>
#include <geometryEngine/dtOCCCurve.h>
#include <geometryEngine/dtOCCBSplineCurve.h>
#include <geometryEngine/dtOCCBezierCurve.h>
#include <geometryEngine/dtOCCSurfaceBase.h>
#include <geometryEngine/dtOCCSurface.h>

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
		dt__ptrAss( 
      dtOCCSurface const * const ss, dtOCCSurface::ConstDownCast(dtS) 
    );
		dt__ptrAss( dtOCCCurve const * const cc, dtOCCCurve::ConstDownCast(dtC) );
		
		Geom_Surface const * sP;
		dt__mustCast(ss->OCCRef().getOCC().get(), Geom_Surface const, sP);
		Geom_Curve const * cP;
		dt__mustCast(cc->OCCRef().getOCC().get(), Geom_Curve const, cP);
		Handle(Geom_Curve) cH = cP;
		Handle(Geom_Surface) sH = sP;
		
		Handle(Geom_Curve) pC = GeomProjLib::Project(cH, sH);
		
		if (pC.IsNull()) {
			dt__throw(
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
			dt__throw(
			  geomCurve_projectOnGeomSurfaceOCC(),
				<< dt__eval(dtOCCBezierCurve::ConstDownCast(dtC)) << std::endl
				<< dt__eval(dtOCCBSplineCurve::ConstDownCast(dtC))
			);
		}
	}

	geomCurve_projectOnGeomSurfaceOCC::~geomCurve_projectOnGeomSurfaceOCC() {
		
	}
	
	dtCurve * geomCurve_projectOnGeomSurfaceOCC::result( void ) {
		return _dtC->clone();		
	}
}
