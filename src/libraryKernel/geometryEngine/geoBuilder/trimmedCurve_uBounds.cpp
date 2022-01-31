#include "trimmedCurve_uBounds.h"

#include <logMe/logMe.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/calculationTypeHandling.h>
#include <geometryEngine/dtOCCCurveBase.h>
#include <geometryEngine/dtOCCCurve.h>
#include <geometryEngine/dtOCCTrimmedCurve.h>

#include <Standard_TypeDef.hxx>
#include <Geom_TrimmedCurve.hxx>

namespace dtOO {
  trimmedCurve_uBounds::trimmedCurve_uBounds(
	  dtCurve const * dtC, dtReal const & min, dtReal const & max
	) {
    dt__ptrAss(
		  dtOCCCurve const * occC, 
			dtOCCCurve::ConstDownCast(dtC)
		);
		
    Standard_Real U1
    = 
    calculationTypeHandling< Standard_Real, dtReal >::boundToRange(
      min, dtC->getUMin(), dtC->getUMax()
    );
    Standard_Real U2
    = 
    calculationTypeHandling< Standard_Real, dtReal >::boundToRange(
      max, dtC->getUMin(), dtC->getUMax()
    );    
		Handle(Geom_TrimmedCurve) ts
      =
      new Geom_TrimmedCurve(
        occC->OCCRef().getOCC(), U1, U2
      );
		
		dtOCCCurveBase base;
		base.setOCC(ts);

		_dtC.reset( new dtOCCTrimmedCurve(base) );		
	}

  dtCurve * trimmedCurve_uBounds::result( void ) {
		return _dtC->clone();
	}

	trimmedCurve_uBounds::~trimmedCurve_uBounds() {
	}
}
