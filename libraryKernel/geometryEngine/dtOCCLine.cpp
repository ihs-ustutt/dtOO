#include "dtOCCLine.h"

#include <logMe/logMe.h>
#include "dtOCCCurveBase.h"
#include "progHelper.h"

#include <Standard_TypeDef.hxx>
#include <gp_Pnt.hxx>
#include <Geom_Line.hxx>

namespace dtOO {				
	dtOCCLine::dtOCCLine() : dtOCCCurve() {
		_ptr = NULL;
	}

	dtOCCLine::dtOCCLine( dtOCCCurveBase const & orig) : dtOCCCurve(orig) {
		dt__mustCast(OCCRef().getOCC().Access(), Geom_Line const, _ptr);
	}
	
	dtOCCLine::~dtOCCLine() {
	}

  dtCurve * dtOCCLine::clone( void ) const {
		return new dtOCCLine( OCCRef() );
	}
	
  int dtOCCLine::order( void ) const {
		dt__functionNotImplemented(order);		
	}
	
  int dtOCCLine::nControlPoints( void ) const {
		dt__functionNotImplemented(nControlPoints);		
	}
	
  dtPoint3 dtOCCLine::controlPoint( int const nPoint ) const {
		dt__functionNotImplemented(controlPoint);		
	}
	
  void dtOCCLine::setControlPoint( int const nPoint, dtPoint3 const point ) {
		dt__functionNotImplemented(setControlPoint);
	}
}
