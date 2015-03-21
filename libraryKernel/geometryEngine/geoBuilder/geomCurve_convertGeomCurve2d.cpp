#include "geomCurve_convertGeomCurve2d.h"
#include <progHelper.h>
#include <logMe/logMe.h>

#include <geometryEngine/dtOCCCurve2dBase.h>
#include <geometryEngine/dtOCCCurveBase.h>
#include <geometryEngine/dtCurve.h>
#include <geometryEngine/dtCurve2d.h>
#include <geometryEngine/dtOCCCurve2d.h>
#include <geometryEngine/dtOCCBSplineCurve.h>
#include <geometryEngine/dtOCCBezierCurve.h>
#include <geometryEngine/dtOCCTrimmedCurve2d.h>
#include <geometryEngine/dtOCCTrimmedCurve.h>
#include <geometryEngine/dtOCCLine.h>

#include <Standard_Failure.hxx>
#include <Standard_ErrorHandler.hxx>
#include <Standard_TypeDef.hxx>
#include <GeomLib.hxx>
#include <gp_Ax2.hxx>
#include <gp_Pnt.hxx>
#include <gp_Dir.hxx>
#include <Geom_Curve.hxx>
#include <Geom_BSplineCurve.hxx>
#include <Geom_BezierCurve.hxx>
#include <Geom_TrimmedCurve.hxx>
#include <Geom_Line.hxx>

//#include <TColStd_Array1OfReal.hxx>
//#include <TColStd_Array1OfInteger.hxx>

namespace dtOO {
	geomCurve_convertGeomCurve2d::geomCurve_convertGeomCurve2d( dtCurve2d const * const dtC2d, dtPoint3 const & pp, dtVector3 const & vv ) {
		dt__ptrAss(dtOCCCurve2d const * const occC2d, dtOCCCurve2d::ConstDownCast(dtC2d));
		
		gp_Ax2 ax2(gp_Pnt(pp.x(), pp.y(), pp.z()), gp_Dir(vv.x(), vv.y(), vv.z()));
		Handle(Geom_Curve) occC = GeomLib::To3d(ax2, occC2d->OCCRef().getOCC());
		
		dtOCCCurveBase base;
		base.setOCC( occC );
		if ( !(Handle(Geom_BSplineCurve)::DownCast(occC).IsNull()) ) {
		  _dtC.reset( new dtOCCBSplineCurve(base) );			
		}
		else if ( !(Handle(Geom_BezierCurve)::DownCast(occC).IsNull()) ) {
		  _dtC.reset( new dtOCCBezierCurve(base) );			
		}
		else if ( !(Handle(Geom_TrimmedCurve)::DownCast(occC).IsNull()) ) {
			dt__ptrAss(
				dtOCCTrimmedCurve2d const * const tC2d, 
				dtOCCTrimmedCurve2d::ConstDownCast(dtC2d)
			);
		  _dtC.reset( new dtOCCTrimmedCurve(base, tC2d->getU0(), tC2d->getU1() ) );
		}		
		else if ( !(Handle(Geom_Line)::DownCast(occC).IsNull()) ) {
		  _dtC.reset( new dtOCCLine(base) );			
		}				
		else {
			dt__THROW(
			  geomCurve_convertGeomCurve2d(), 
			  << "Unknown OCC type."
			);
		}
	}
	
	geomCurve_convertGeomCurve2d::~geomCurve_convertGeomCurve2d() {
	}
	
	dtCurve * geomCurve_convertGeomCurve2d::result( void ) {
		return _dtC->clone();
	}
}