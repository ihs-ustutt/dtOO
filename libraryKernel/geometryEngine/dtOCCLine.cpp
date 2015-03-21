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
		dt__mustDownCast(OCCRef().getOCC().Access(), Geom_Line const, _ptr);
	}
	
	dtOCCLine::~dtOCCLine() {
	}

  dtCurve * dtOCCLine::clone( void ) const {
		return new dtOCCLine( OCCRef() );
	}
	
  int dtOCCLine::order( void ) const {
		DTFUNCTIONNOTI(order);		
	}
	
  int dtOCCLine::nControlPoints( void ) const {
		DTFUNCTIONNOTI(nControlPoints);		
	}
	
  dtPoint3 dtOCCLine::controlPoint( int const nPoint ) const {
		DTFUNCTIONNOTI(controlPoint);		
	}
	
  void dtOCCLine::setControlPoint( int const nPoint, dtPoint3 const point ) {
		DTFUNCTIONNOTI(setControlPoint);
	}
}
