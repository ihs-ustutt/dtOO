#include "dtOCCBSplineSurface.h"

#include <logMe/logMe.h>
#include "dtOCCSurfaceBase.h"
#include "dtOCCCurveBase.h"
#include "dtOCCBSplineCurve.h"
#include <progHelper.h>

#include <Standard_TypeDef.hxx>
#include <gp_Pnt.hxx>
#include <Geom_BSplineSurface.hxx>
#include <Geom_BSplineCurve.hxx>

namespace dtOO {	
	dtOCCBSplineSurface::dtOCCBSplineSurface() : dtOCCSurface() {
		_ptr = NULL;
	}

	dtOCCBSplineSurface::dtOCCBSplineSurface(const dtOCCSurfaceBase& orig) 
		: dtOCCSurface(orig) {
		dt__MUSTDOWNCAST(OCCRef().getOCC().Access(), Geom_BSplineSurface const, _ptr);
	}

	dtOCCBSplineSurface::~dtOCCBSplineSurface() {

	}

	dtSurface * dtOCCBSplineSurface::clone( void ) const {
		return new dtOCCBSplineSurface( OCCRef() );
	}

	dtPoint3 dtOCCBSplineSurface::controlPoint( int const uI, int const vI ) const {
		Standard_Integer uSi = static_cast<Standard_Integer>(uI+1);		
		Standard_Integer vSi = static_cast<Standard_Integer>(vI+1);		
		gp_Pnt pp = _ptr->Pole(uSi, vSi);

		return dtPoint3(
						static_cast<float>(pp.Coord(1)), 
						static_cast<float>(pp.Coord(2)), 
						static_cast<float>(pp.Coord(3))
		);				
	}

	void dtOCCBSplineSurface::setControlPoint( int const uI, int const vI, dtPoint3 const point ) {
		gp_Pnt pp(point.x(), point.y(), point.z());
		const_cast<Geom_BSplineSurface*>(_ptr)->SetPole(uI+1, vI+1, pp);
	}

	int dtOCCBSplineSurface::nControlPoints( int const dim ) const {
		switch (dim) {
			case 0:		
				return static_cast<int>(_ptr->NbUPoles());					
			case 1:		
				return static_cast<int>(_ptr->NbVPoles());
			default:
				dt__THROW(nControlPoints(),
							<< DTLOGEVAL(dim) << LOGDEL
							<< "dim should be 0 or 1.");				
		}	
	}

	dtCurve * dtOCCBSplineSurface::getCurveConstU( float const uu, float const vvMin, float const vvMax) const {
		Standard_Real uR = static_cast<Standard_Real>(uu);
		Handle(Geom_Curve) cc = _ptr->UIso(uR);

		Handle(Geom_BSplineCurve) ccB = Handle(Geom_BSplineCurve)::DownCast(cc);

		Standard_Real v1R = static_cast<Standard_Real>(vvMin);
		Standard_Real v2R = static_cast<Standard_Real>(vvMax);

		ccB->Segment(v1R, v2R);

		dtOCCCurveBase base;
		base.setOCC(ccB);

		return new dtOCCBSplineCurve(base);
	}

	dtCurve * dtOCCBSplineSurface::getCurveConstV( float const vv, float const uuMin, float const uuMax) const {
		Standard_Real vR = static_cast<Standard_Real>(vv);
		Handle(Geom_Curve) cc = _ptr->VIso(vR);

		Handle(Geom_BSplineCurve) ccB = Handle(Geom_BSplineCurve)::DownCast(cc);

		Standard_Real u1R = static_cast<Standard_Real>(uuMin);
		Standard_Real u2R = static_cast<Standard_Real>(uuMax);

		ccB->Segment(u1R, u2R);

		dtOCCCurveBase base;
		base.setOCC(ccB);

		return new dtOCCBSplineCurve(base);
	}		
}
