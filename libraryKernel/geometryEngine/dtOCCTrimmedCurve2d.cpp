#include "dtOCCTrimmedCurve2d.h"

#include <logMe/logMe.h>
#include "dtOCCCurve2dBase.h"
#include "progHelper.h"

#include <Standard_TypeDef.hxx>
//#include <gp_Pnt2d.hxx>
#include <Geom2d_TrimmedCurve.hxx>

namespace dtOO {
	dtOCCTrimmedCurve2d::dtOCCTrimmedCurve2d() : dtOCCCurve2d() {
		_ptr = NULL;		
	}

	dtOCCTrimmedCurve2d::dtOCCTrimmedCurve2d(dtOCCCurve2dBase const & orig) : dtOCCCurve2d(orig) {
		dt__MUSTDOWNCAST(OCCRef().getOCC().Access(), Geom2d_TrimmedCurve const, _ptr);		
	}

	dtOCCTrimmedCurve2d::~dtOCCTrimmedCurve2d() {
	}

  dtCurve2d * dtOCCTrimmedCurve2d::clone( void ) const {
		return new dtOCCTrimmedCurve2d( OCCRef() );		
	}
	
  int dtOCCTrimmedCurve2d::order( void ) const {
		DTFUNCTIONNOTI(order());			
	}
	
  int dtOCCTrimmedCurve2d::nControlPoints( void ) const {
		return 0;			
	}
	
  dtPoint2 dtOCCTrimmedCurve2d::controlPoint( int const nPoint ) const {
		DTFUNCTIONNOTI(controlPoint());		
	}
		
}
