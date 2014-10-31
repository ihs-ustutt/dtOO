#include "dtOCCBSplineCurve.h"

#include <logMe/logMe.h>
#include "dtOCCCurveBase.h"
#include "progHelper.h"

#include <Standard_TypeDef.hxx>
#include <gp_Pnt.hxx>
#include <Geom_BSplineCurve.hxx>

namespace dtOO {					
	dtOCCBSplineCurve::dtOCCBSplineCurve() : dtOCCCurve() {
		_ptr = NULL;
	}

	dtOCCBSplineCurve::dtOCCBSplineCurve( dtOCCCurveBase const & orig) : dtOCCCurve(orig) {
		dt__MUSTDOWNCAST(OCCRef().getOCC().Access(), Geom_BSplineCurve const, _ptr);
	}
	
	dtOCCBSplineCurve::~dtOCCBSplineCurve() {
	}

  dtCurve * dtOCCBSplineCurve::clone( void ) const {
		return new dtOCCBSplineCurve( OCCRef() );
	}
	
  int dtOCCBSplineCurve::getOrder( void ) const {
    return static_cast<int>(_ptr->Degree());		
	}
	
  int dtOCCBSplineCurve::getNControlPoints( void ) const {
		return static_cast<int>(_ptr->NbPoles());		
	}
	
  dtPoint3 dtOCCBSplineCurve::getControlPoint3d( int const nPoint ) const {
		Standard_Integer nPI = static_cast<Standard_Integer>(nPoint+1);		
		gp_Pnt pp = _ptr->Pole(nPI);
		
		return dtPoint3(
						static_cast<float>(pp.Coord(1)), 
						static_cast<float>(pp.Coord(2)), 
						static_cast<float>(pp.Coord(3))
		);		
	}
	
  void dtOCCBSplineCurve::setControlPoint3d( int const nPoint, dtPoint3 const point ) {
		DTFUNCTIONNOTI(setControlPoint3d);
	}
}
