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
		dt__MUSTDOWNCAST(OCCRef().getOCC().Access(), Geom_Line const, _ptr);
	}
	
	dtOCCLine::~dtOCCLine() {
	}

  dtCurve * dtOCCLine::clone( void ) const {
		return new dtOCCLine( OCCRef() );
	}
	
  int dtOCCLine::getOrder( void ) const {
		DTFUNCTIONNOTI(getOrder);		
	}
	
  int dtOCCLine::getNControlPoints( void ) const {
		DTFUNCTIONNOTI(getNControlPoints);		
	}
	
  dtPoint3 dtOCCLine::getControlPoint3d( int const nPoint ) const {
		DTFUNCTIONNOTI(getControlPoint3d);		
	}
	
  void dtOCCLine::setControlPoint3d( int const nPoint, dtPoint3 const point ) {
		DTFUNCTIONNOTI(setControlPoint3d);
	}
}
