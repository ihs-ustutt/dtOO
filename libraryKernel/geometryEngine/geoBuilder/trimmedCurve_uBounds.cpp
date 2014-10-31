#include "trimmedCurve_uBounds.h"

#include <logMe/logMe.h>
#include <progHelper.h>
#include <geometryEngine/dtOCCCurveBase.h>
#include <geometryEngine/dtOCCCurve.h>
#include <geometryEngine/dtOCCTrimmedCurve.h>

#include <Standard_TypeDef.hxx>
#include <Geom_TrimmedCurve.hxx>

namespace dtOO {
  trimmedCurve_uBounds::trimmedCurve_uBounds(
	  dtCurve const * dtC, float const & min, float const & max
	) {
    dt__PTRASS(
		  dtOCCCurve const * occC, 
			dtOCCCurve::ConstDownCast(dtC)
		);
		
		Standard_Real U1 = static_cast<Standard_Real>(min);
		Standard_Real U2 = static_cast<Standard_Real>(max);

		Handle(Geom_TrimmedCurve) ts
		=
		new Geom_TrimmedCurve(
		  occC->OCCRef().getOCC(), U1, U2
		);
		
		dtOCCCurveBase base;
		base.setOCC(ts);

		_dtC.reset( new dtOCCTrimmedCurve(base, min, max) );		
	}

  dtCurve * trimmedCurve_uBounds::result( void ) {
		return _dtC->clone();
	}

	trimmedCurve_uBounds::~trimmedCurve_uBounds() {
	}
}
