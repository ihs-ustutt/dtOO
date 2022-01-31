#include "dtOCCEllipse.h"

#include <logMe/logMe.h>
#include "dtOCCCurveBase.h"
#include "progHelper.h"

#include <Standard_TypeDef.hxx>
#include <gp_Pnt.hxx>
#include <Geom_Ellipse.hxx>
#include <Precision.hxx>

namespace dtOO {
	dtOCCEllipse::dtOCCEllipse() : dtOCCCurve() {
		_ptr = NULL;		
	}

	dtOCCEllipse::dtOCCEllipse(dtOCCCurveBase const & orig) : dtOCCCurve(orig) {
		dt__mustCast(OCCRef().getOCC().Access(), Geom_Ellipse const, _ptr);	
	}

	dtOCCEllipse::~dtOCCEllipse() {
	}

  dtCurve * dtOCCEllipse::clone( void ) const {
		return new dtOCCEllipse( OCCRef() );		
	}
	
  dtInt dtOCCEllipse::order( void ) const {
		dt__throwUnexpected(order());			
	}
	
  dtInt dtOCCEllipse::nControlPoints( void ) const {
		return 0;
	}
	
  dtPoint3 dtOCCEllipse::controlPoint( dtInt const nPoint ) const {
		dt__throwUnexpected(controlPoint());		
	}
	
  void dtOCCEllipse::setControlPoint( dtInt const nPoint, dtPoint3 const point ) {
		dt__throwUnexpected(setControlPoint());				
	}
}
