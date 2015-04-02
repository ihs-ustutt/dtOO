#include "dtOCCRectangularTrimmedSurface.h"

#include <logMe/logMe.h>
#include "dtOCCSurfaceBase.h"
#include "dtOCCCurveBase.h"
#include "dtOCCBezierCurve.h"
#include "dtOCCTrimmedCurve.h"
#include <progHelper.h>

#include <Standard_Failure.hxx>
#include <Standard_TypeDef.hxx>
#include <Geom_RectangularTrimmedSurface.hxx>
#include <Geom_TrimmedCurve.hxx>

namespace dtOO {
	dtOCCRectangularTrimmedSurface::dtOCCRectangularTrimmedSurface() : dtOCCSurface() {
		_ptr = NULL;
	}

	dtOCCRectangularTrimmedSurface::dtOCCRectangularTrimmedSurface(const dtOCCSurfaceBase& orig) 
		: dtOCCSurface(orig) {
		dt__mustCast(OCCRef().getOCC().Access(), Geom_RectangularTrimmedSurface const, _ptr);
	}

	dtOCCRectangularTrimmedSurface::~dtOCCRectangularTrimmedSurface() {

	}

	dtSurface * dtOCCRectangularTrimmedSurface::clone( void ) const {
		return new dtOCCRectangularTrimmedSurface( OCCRef() );
	}

	dtCurve * dtOCCRectangularTrimmedSurface::segmentConstU( float const uu, float const vvMin, float const vvMax) const {
		Standard_Real uR = static_cast<Standard_Real>(uu);
		Handle(Geom_Curve) cc = _ptr->UIso(uR);

		Handle(Geom_TrimmedCurve) ccB = Handle(Geom_TrimmedCurve)::DownCast(cc);

		Standard_Real v1R = static_cast<Standard_Real>(vvMin);
		Standard_Real v2R = static_cast<Standard_Real>(vvMax);

		ccB->SetTrim(v1R, v2R);

		dtOCCCurveBase base;
		base.setOCC(ccB);

		return new dtOCCTrimmedCurve(base, vvMin, vvMax);
	}

	dtCurve * dtOCCRectangularTrimmedSurface::segmentConstV( float const vv, float const uuMin, float const uuMax) const {
			Standard_Real vR = static_cast<Standard_Real>(vv);
			Handle(Geom_Curve) cc = _ptr->VIso(vR);

			Handle(Geom_TrimmedCurve) ccB = Handle(Geom_TrimmedCurve)::DownCast(cc);

			Standard_Real u1R = static_cast<Standard_Real>(uuMin);
			Standard_Real u2R = static_cast<Standard_Real>(uuMax);

			ccB->SetTrim(u1R, u2R);

			dtOCCCurveBase base;
			base.setOCC(ccB);

			return new dtOCCTrimmedCurve(base, uuMin, uuMax);
	}		
}
