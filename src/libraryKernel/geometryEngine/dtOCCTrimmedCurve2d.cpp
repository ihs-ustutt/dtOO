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

	dtOCCTrimmedCurve2d::dtOCCTrimmedCurve2d(
    dtOCCCurve2dBase const & orig, float const u0, float const u1
  ) : dtOCCCurve2d(orig) {
		dt__mustCast(OCCRef().getOCC().Access(), Geom2d_TrimmedCurve const, _ptr);		
		_u0 = u0;
		_u1 = u1;
	}

	dtOCCTrimmedCurve2d::~dtOCCTrimmedCurve2d() {
	}

  dtCurve2d * dtOCCTrimmedCurve2d::clone( void ) const {
		return new dtOCCTrimmedCurve2d( OCCRef(), _u0, _u1 );		
	}
	
  int dtOCCTrimmedCurve2d::order( void ) const {
		dt__throwUnexpected(order());			
	}
	
  int dtOCCTrimmedCurve2d::nControlPoints( void ) const {
		return 0;			
	}
	
  dtPoint2 dtOCCTrimmedCurve2d::controlPoint( int const nPoint ) const {
		dt__throwUnexpected(controlPoint());		
	}
	
  float dtOCCTrimmedCurve2d::getU0( void ) const {
		return _u0;
	}
	
  float dtOCCTrimmedCurve2d::getU1( void ) const {
		return _u1;
	}
}
