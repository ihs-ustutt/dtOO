#include "dtOCCBezierSurface.h"

#include <logMe/logMe.h>
#include "dtOCCSurfaceBase.h"
#include "dtOCCCurveBase.h"
#include "dtOCCBezierCurve.h"
#include <progHelper.h>

#include <Standard_TypeDef.hxx>
#include <gp_Pnt.hxx>
#include <Geom_BezierSurface.hxx>
#include <Geom_BezierCurve.hxx>

namespace dtOO {	
	dtOCCBezierSurface::dtOCCBezierSurface() : dtOCCSurface() {
		_ptr = NULL;
	}

	dtOCCBezierSurface::dtOCCBezierSurface(const dtOCCSurfaceBase& orig) 
		: dtOCCSurface(orig) {
		dt__mustCast(OCCRef().getOCC().Access(), Geom_BezierSurface const, _ptr);
	}

	dtOCCBezierSurface::~dtOCCBezierSurface() {

	}

	dtSurface * dtOCCBezierSurface::clone( void ) const {
		return new dtOCCBezierSurface( OCCRef() );
	}

	dtPoint3 dtOCCBezierSurface::controlPoint( int const uI, int const vI ) const {
		Standard_Integer uSi = static_cast<Standard_Integer>(uI+1);		
		Standard_Integer vSi = static_cast<Standard_Integer>(vI+1);		
		gp_Pnt pp = _ptr->Pole(uSi, vSi);

		return dtPoint3(
						static_cast<float>(pp.Coord(1)), 
						static_cast<float>(pp.Coord(2)), 
						static_cast<float>(pp.Coord(3))
		);				
	}

	void dtOCCBezierSurface::setControlPoint( int const uI, int const vI, dtPoint3 const point ) {
		dt__functionNotImplemented(setControlPoint);			
	}

	int dtOCCBezierSurface::nControlPoints( int const dim ) const {
		switch (dim) {
			case 0:		
				return static_cast<int>(_ptr->NbUPoles());					
			case 1:		
				return static_cast<int>(_ptr->NbVPoles());
			default:
				dt__throw(getNControlPoints(),
							<< dt__eval(dim) << std::endl
							<< "dim should be 0 or 1.");				
		}	
	}

	dtCurve * dtOCCBezierSurface::segmentConstU( float const uu, float const vvMin, float const vvMax) const {
		Standard_Real uR = static_cast<Standard_Real>(uu);
		Handle(Geom_Curve) cc = _ptr->UIso(uR);

		Handle(Geom_BezierCurve) ccB = Handle(Geom_BezierCurve)::DownCast(cc);

		Standard_Real v1R = static_cast<Standard_Real>(vvMin);
		Standard_Real v2R = static_cast<Standard_Real>(vvMax);

		ccB->Segment(v1R, v2R);

		dtOCCCurveBase base;
		base.setOCC(ccB);

		return new dtOCCBezierCurve(base);
	}

	dtCurve * dtOCCBezierSurface::segmentConstV( float const vv, float const uuMin, float const uuMax) const {
		Standard_Real vR = static_cast<Standard_Real>(vv);
		Handle(Geom_Curve) cc = _ptr->VIso(vR);

		Handle(Geom_BezierCurve) ccB = Handle(Geom_BezierCurve)::DownCast(cc);

		Standard_Real u1R = static_cast<Standard_Real>(uuMin);
		Standard_Real u2R = static_cast<Standard_Real>(uuMax);

		ccB->Segment(u1R, u2R);

		dtOCCCurveBase base;
		base.setOCC(ccB);

		return new dtOCCBezierCurve(base);
	}		
}
