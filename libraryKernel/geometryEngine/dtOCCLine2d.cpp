#include "dtOCCLine2d.h"

#include <logMe/logMe.h>
#include "dtOCCCurve2dBase.h"
#include "progHelper.h"

#include <Standard_TypeDef.hxx>
//#include <gp_Pnt2d.hxx>
#include <Geom2d_Line.hxx>

namespace dtOO {
	dtOCCLine2d::dtOCCLine2d() : dtOCCCurve2d() {
		_ptr = NULL;		
	}

	dtOCCLine2d::dtOCCLine2d(dtOCCCurve2dBase const & orig) : dtOCCCurve2d(orig) {
		dt__mustCast(OCCRef().getOCC().Access(), Geom2d_Line const, _ptr);		
	}

	dtOCCLine2d::~dtOCCLine2d() {
	}

  dtCurve2d * dtOCCLine2d::clone( void ) const {
		return new dtOCCLine2d( OCCRef() );		
	}
	
  int dtOCCLine2d::order( void ) const {
		dt__functionNotImplemented(order());			
	}
	
  int dtOCCLine2d::nControlPoints( void ) const {
		return 0;			
	}
	
  dtPoint2 dtOCCLine2d::controlPoint( int const nPoint ) const {
		dt__functionNotImplemented(controlPoint());		
	}
		
}
