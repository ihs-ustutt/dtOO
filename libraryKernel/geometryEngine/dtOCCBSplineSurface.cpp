#include "dtOCCBSplineSurface.h"

#include <logMe/logMe.h>
#include "dtOCCSurfaceBase.h"
#include "dtOCCCurveBase.h"
#include "dtOCCBSplineCurve.h"
#include <progHelper.h>

#include <Standard_Failure.hxx>
#include <Standard_ErrorHandler.hxx>
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
		dt__mustDownCast(OCCRef().getOCC().Access(), Geom_BSplineSurface const, _ptr);
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
		Standard_Integer uSi = static_cast<Standard_Integer>(uI+1);		
		Standard_Integer vSi = static_cast<Standard_Integer>(vI+1);		
		
		gp_Pnt pp(point.x(), point.y(), point.z());

		Handle(Geom_Surface) occS = dtOCCSurface::OCCRef().getOCC();
		Handle(Geom_BSplineSurface) occBss = Handle(Geom_BSplineSurface)::DownCast(occS);

		occBss->SetPole(uSi, vSi, pp);
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

	dtCurve * dtOCCBSplineSurface::segmentConstU( float const uu, float const vvMin, float const vvMax) const {
		Standard_Real uR = static_cast<Standard_Real>(uu);
		Handle(Geom_Curve) cc = _ptr->UIso(uR);

		Handle(Geom_BSplineCurve) ccB = Handle(Geom_BSplineCurve)::DownCast(cc);

		Standard_Real v1R = static_cast<Standard_Real>(vvMin);
		Standard_Real v2R = static_cast<Standard_Real>(vvMax);

    dt__WARN_IFWMAS(
			v1R<ccB->FirstParameter(), 
			v1R = ccB->FirstParameter();,
			segmentConstU(),
			<< "v1R = ccB->FirstParameter();"				
		);			
    dt__WARN_IFWMAS(
			v2R>ccB->LastParameter(), 
			v2R = ccB->LastParameter();,
			segmentConstU(),
			<< "v2R = ccB->LastParameter();"				
		);	
			
		dt__TRYOCC(
		  ccB->Segment(v1R, v2R);
		  ,
		  << DTLOGEVAL(v1R) << LOGDEL
			<< DTLOGEVAL(v2R)							
	  );

		dtOCCCurveBase base;
		base.setOCC(ccB);

		return new dtOCCBSplineCurve(base);
	}

	dtCurve * dtOCCBSplineSurface::segmentConstV( float const vv, float const uuMin, float const uuMax) const {
		Standard_Real vR = static_cast<Standard_Real>(vv);
		Handle(Geom_Curve) cc = _ptr->VIso(vR);

		Handle(Geom_BSplineCurve) ccB = Handle(Geom_BSplineCurve)::DownCast(cc);

		Standard_Real u1R = static_cast<Standard_Real>(uuMin);
		Standard_Real u2R = static_cast<Standard_Real>(uuMax);

    dt__WARN_IFWMAS(
			u1R<ccB->FirstParameter(), 
			u1R = ccB->FirstParameter();,
			segmentConstV(),
			<< "u1R = ccB->FirstParameter();"				
		);			
    dt__WARN_IFWMAS(
			u2R>ccB->LastParameter(), 
			u2R = ccB->LastParameter();,
			segmentConstV(),
			<< "u2R = ccB->LastParameter();"				
		);	
							
		dt__TRYOCC(
		  ccB->Segment(u1R, u2R);
		  ,
		  << DTLOGEVAL(u1R) << LOGDEL
			<< DTLOGEVAL(u2R)							
	  );

		dtOCCCurveBase base;
		base.setOCC(ccB);

		return new dtOCCBSplineCurve(base);
	}		
}
