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
    dtOCCCurveBase const & orig
  ) : dtOCCCurve(orig) {
		dt__mustCast(OCCRef().getOCC().Access(), Geom_TrimmedCurve const, _ptr);
	}  

	dtOCCTrimmedCurve::~dtOCCTrimmedCurve() {
	}

  dtCurve * dtOCCTrimmedCurve::clone( void ) const {
		return new dtOCCTrimmedCurve( OCCRef());		
	}
	
  dtInt dtOCCTrimmedCurve::order( void ) const {
		dt__throwUnexpected(order());			
	}
	
  dtInt dtOCCTrimmedCurve::nControlPoints( void ) const {
		return 0;			
	}
	
  dtPoint3 dtOCCTrimmedCurve::controlPoint( dtInt const nPoint ) const {
		dt__throwUnexpected(controlPoint());		
	}
	
  void dtOCCTrimmedCurve::setControlPoint( 
    dtInt const nPoint, dtPoint3 const point 
  ) {
		dt__throwUnexpected(setControlPoint());				
	}
}
