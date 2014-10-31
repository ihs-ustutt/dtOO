#include "dtOCCBezierCurve.h"

#include <logMe/logMe.h>
#include "dtOCCCurveBase.h"
#include "progHelper.h"

#include <Standard_TypeDef.hxx>
#include <gp_Pnt.hxx>
#include <Geom_BezierCurve.hxx>

namespace dtOO {				
	dtOCCBezierCurve::dtOCCBezierCurve() : dtOCCCurve() {
		_ptr = NULL;
	}

	dtOCCBezierCurve::dtOCCBezierCurve( dtOCCCurveBase const & orig) : dtOCCCurve(orig) {
		dt__MUSTDOWNCAST(OCCRef().getOCC().Access(), Geom_BezierCurve const, _ptr);
	}
	
	dtOCCBezierCurve::~dtOCCBezierCurve() {
	}

  dtCurve * dtOCCBezierCurve::clone( void ) const {
		return new dtOCCBezierCurve( OCCRef() );
	}
	
  int dtOCCBezierCurve::getOrder( void ) const {
    return static_cast<int>(_ptr->Degree());		
	}
	
  int dtOCCBezierCurve::getNControlPoints( void ) const {
		return static_cast<int>(_ptr->NbPoles());		
	}
	
  dtPoint3 dtOCCBezierCurve::getControlPoint3d( int const nPoint ) const {
		Standard_Integer nPI = static_cast<Standard_Integer>(nPoint+1);		
		gp_Pnt pp = _ptr->Pole(nPI);
		
		return dtPoint3(
						static_cast<float>(pp.Coord(1)), 
						static_cast<float>(pp.Coord(2)), 
						static_cast<float>(pp.Coord(3))
		);		
	}
	
  void dtOCCBezierCurve::setControlPoint3d( int const nPoint, dtPoint3 const point ) {
		DTFUNCTIONNOTI(setControlPoint3d);
	}
}
