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

#include "bezierSurface_bezierCurveFillConstructOCC.h"

#include <logMe/logMe.h>
#include <geometryEngine/dtOCCSurfaceBase.h>
#include <geometryEngine/dtOCCCurveBase.h>
#include <geometryEngine/dtOCCBezierCurve.h>
#include <geometryEngine/dtOCCBezierSurface.h>

#include <Geom_BezierCurve.hxx>
#include <Geom_BezierSurface.hxx>
#include <GeomFill_FillingStyle.hxx>
#include <GeomFill_BezierCurves.hxx>

namespace dtOO {
	bezierSurface_bezierCurveFillConstructOCC::bezierSurface_bezierCurveFillConstructOCC( 
	  vectorHandling< dtCurve const * > const & cc 
	) {
		if (cc.size() != 4) {
			dt__throw(
				bezierSurface_bezierCurveFillConstructOCC(),
				<< "Only supported with 4 curves." << std::endl
				<< dt__eval(cc.size())
			);
		}
		Handle(Geom_BezierCurve) C1;
		Handle(Geom_BezierCurve) C2;
		Handle(Geom_BezierCurve) C3;
		Handle(Geom_BezierCurve) C4;
	  
		dtOCCBezierCurve const * occC;
	  dt__ptrAss( occC, dtOCCBezierCurve::ConstDownCast(cc[0]) );
		C1 = Handle(Geom_BezierCurve)::DownCast( occC->OCCRef().getOCC() );
	  dt__ptrAss( occC, dtOCCBezierCurve::ConstDownCast(cc[1]) );
		C2 = Handle(Geom_BezierCurve)::DownCast( occC->OCCRef().getOCC() );
	  dt__ptrAss( occC, dtOCCBezierCurve::ConstDownCast(cc[2]) );
		C3 = Handle(Geom_BezierCurve)::DownCast( occC->OCCRef().getOCC() );
	  dt__ptrAss( occC, dtOCCBezierCurve::ConstDownCast(cc[3]) );
		C4 = Handle(Geom_BezierCurve)::DownCast( occC->OCCRef().getOCC() );		
		
		GeomFill_BezierCurves fill(
      C1, C2, C3, C4, GeomFill_FillingStyle::GeomFill_StretchStyle
    );
		
		dtOCCSurfaceBase base;
		base.setOCC( Handle(Geom_Surface)::DownCast(fill.Surface()) );
		_dtS.reset( new dtOCCBezierSurface(base) );		
	}
	
	bezierSurface_bezierCurveFillConstructOCC::~bezierSurface_bezierCurveFillConstructOCC() {
	}
	
	dtSurface * bezierSurface_bezierCurveFillConstructOCC::result( void ) {
		return _dtS->clone();
	}
}
