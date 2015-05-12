#include "dtOCCCompositeSurface.h"

#include <logMe/logMe.h>
#include "dtOCCSurfaceBase.h"
#include "dtOCCCurveBase.h"
#include "dtOCCBSplineCurve.h"
#include "dtOCCBezierCurve.h"
#include <progHelper.h>

#include <Standard_Failure.hxx>
#include <Standard_ErrorHandler.hxx>
#include <Standard_TypeDef.hxx>
#include <gp_Pnt.hxx>
#include <ShapeExtend_CompositeSurface.hxx>
#include <Geom_BSplineCurve.hxx>
#include <Geom_BezierCurve.hxx>
#include <Geom_Line.hxx>

namespace dtOO {	
	dtOCCCompositeSurface::dtOCCCompositeSurface() : dtOCCSurface() {
		_ptr = NULL;
	}

	dtOCCCompositeSurface::dtOCCCompositeSurface(const dtOCCSurfaceBase& orig) 
		: dtOCCSurface(orig) {
		dt__mustCast(OCCRef().getOCC().Access(), ShapeExtend_CompositeSurface const, _ptr);
	}

	dtOCCCompositeSurface::~dtOCCCompositeSurface() {

	}

	dtSurface * dtOCCCompositeSurface::clone( void ) const {
		return new dtOCCCompositeSurface( OCCRef() );
	}

	dtCurve * dtOCCCompositeSurface::segmentConstU( float const uu, float const vvMin, float const vvMax) const {
		Standard_Real uR = static_cast<Standard_Real>(uu);
		Handle(Geom_Curve) cc = _ptr->UIso(uR);

		Handle(Geom_BSplineCurve) ccB = Handle(Geom_BSplineCurve)::DownCast(cc);
		Handle(Geom_BezierCurve) ccBezier = Handle(Geom_BezierCurve)::DownCast(cc);

		Standard_Real v1R = static_cast<Standard_Real>(vvMin);
		Standard_Real v2R = static_cast<Standard_Real>(vvMax);
			
		if ( !ccB.IsNull() ) {
			dt__warnIfWithMessageAndSolution(
				v1R<ccB->FirstParameter(), 
				v1R = ccB->FirstParameter();,
				segmentConstU(),
				<< "v1R = ccB->FirstParameter();"				
			);			
			dt__warnIfWithMessageAndSolution(
				v2R>ccB->LastParameter(), 
				v2R = ccB->LastParameter();,
				segmentConstU(),
				<< "v2R = ccB->LastParameter();"				
			);	
			
			dt__tryOcc(
				ccB->Segment(v1R, v2R);
				,
				<< dt__eval(v1R) << std::endl
				<< dt__eval(v2R)							
			);

			dtOCCCurveBase base;
			base.setOCC(ccB);

			return new dtOCCBSplineCurve(base);
		}
		else if ( !ccBezier.IsNull() ) {
			dt__warnIfWithMessageAndSolution(
				v1R<ccBezier->FirstParameter(), 
				v1R = ccBezier->FirstParameter();,
				segmentConstU(),
				<< "v1R = ccB->FirstParameter();"				
			);			
			dt__warnIfWithMessageAndSolution(
				v2R>ccBezier->LastParameter(), 
				v2R = ccBezier->LastParameter();,
				segmentConstU(),
				<< "v2R = ccB->LastParameter();"				
			);	
			
			dt__tryOcc(
				ccBezier->Segment(v1R, v2R);
				,
				<< dt__eval(v1R) << std::endl
				<< dt__eval(v2R)							
			);

			dtOCCCurveBase base;
			base.setOCC(ccBezier);

			return new dtOCCBezierCurve(base);			
		}
		else dt__throwUnexpected(segmentConstU());
	}

	dtCurve * dtOCCCompositeSurface::segmentConstV( float const vv, float const uuMin, float const uuMax) const {
		Standard_Real vR = static_cast<Standard_Real>(vv);
		Handle(Geom_Curve) cc = _ptr->VIso(vR);

		Handle(Geom_BSplineCurve) ccB = Handle(Geom_BSplineCurve)::DownCast(cc);
		Handle(Geom_BezierCurve) ccBezier = Handle(Geom_BezierCurve)::DownCast(cc);

		Standard_Real u1R = static_cast<Standard_Real>(uuMin);
		Standard_Real u2R = static_cast<Standard_Real>(uuMax);
			
		if ( !ccB.IsNull() ) {
			dt__warnIfWithMessageAndSolution(
				u1R<ccB->FirstParameter(), 
				u1R = ccB->FirstParameter();,
				segmentConstV(),
				<< "u1R = ccB->FirstParameter();"				
			);			
			dt__warnIfWithMessageAndSolution(
				u2R>ccB->LastParameter(), 
				u2R = ccB->LastParameter();,
				segmentConstV(),
				<< "u2R = ccB->LastParameter();"				
			);	
			
			dt__tryOcc(
				ccB->Segment(u1R, u2R);
				,
				<< dt__eval(u1R) << std::endl
				<< dt__eval(u2R)							
			);

			dtOCCCurveBase base;
			base.setOCC(ccB);

			return new dtOCCBSplineCurve(base);
		}
		else if ( !ccBezier.IsNull() ) {
			dt__warnIfWithMessageAndSolution(
				u1R<ccBezier->FirstParameter(), 
				u1R = ccBezier->FirstParameter();,
				segmentConstV(),
				<< "v1R = ccB->FirstParameter();"				
			);			
			dt__warnIfWithMessageAndSolution(
				u2R>ccBezier->LastParameter(), 
				u2R = ccBezier->LastParameter();,
				segmentConstV(),
				<< "v2R = ccB->LastParameter();"				
			);	
			
			dt__tryOcc(
				ccBezier->Segment(u1R, u2R);
				,
				<< dt__eval(u1R) << std::endl
				<< dt__eval(u2R)							
			);

			dtOCCCurveBase base;
			base.setOCC(ccBezier);

			return new dtOCCBezierCurve(base);			
		}
		else dt__throwUnexpected(segmentConstV());
	}		
}
