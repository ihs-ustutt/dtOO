#include "dtOCCTrimmedCurve.h"

#include <logMe/logMe.h>
#include "dtOCCCurveBase.h"
#include "dtOCCBSplineCurve.h"
#include "progHelper.h"

#include <Standard_TypeDef.hxx>
#include <gp_Pnt.hxx>
#include <Geom_TrimmedCurve.hxx>
#include <Geom_BSplineCurve.hxx>
#include <Geom_Line.hxx>
#include <Precision.hxx>
#include <GeomConvert.hxx>
#include <geometryEngine/geoBuilder/bSplineCurve_pointConstructOCC.h>

namespace dtOO {
	dtOCCTrimmedCurve::dtOCCTrimmedCurve() : dtOCCCurve() {
		_ptr = NULL;		
	}

	dtOCCTrimmedCurve::dtOCCTrimmedCurve(
    dtOCCCurveBase const & orig, float const u0, float const u1
  ) : dtOCCCurve(orig) {
		dt__mustCast(OCCRef().getOCC().Access(), Geom_TrimmedCurve const, _ptr);
		_u0 = u0;
		_u1 = u1;
	}

	dtOCCTrimmedCurve::~dtOCCTrimmedCurve() {
	}

  dtCurve * dtOCCTrimmedCurve::clone( void ) const {
		return new dtOCCTrimmedCurve( OCCRef(), _u0, _u1 );		
	}
	
  int dtOCCTrimmedCurve::order( void ) const {
		dt__throwUnexpected(order());			
	}
	
  int dtOCCTrimmedCurve::nControlPoints( void ) const {
		return 0;			
	}
	
  dtPoint3 dtOCCTrimmedCurve::controlPoint( int const nPoint ) const {
		dt__throwUnexpected(controlPoint());		
	}
	
  void dtOCCTrimmedCurve::setControlPoint( 
    int const nPoint, dtPoint3 const point 
  ) {
		dt__throwUnexpected(setControlPoint());				
	}
}
