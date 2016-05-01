#include "dtOCCBSplineCurve2d.h"

#include <logMe/logMe.h>
#include "dtOCCCurve2dBase.h"
#include "progHelper.h"

#include <Standard_TypeDef.hxx>
#include <gp_Pnt2d.hxx>
#include <Geom2d_BSplineCurve.hxx>

namespace dtOO {					
	dtOCCBSplineCurve2d::dtOCCBSplineCurve2d() : dtOCCCurve2d() {
		_ptr = NULL;
	}

	dtOCCBSplineCurve2d::dtOCCBSplineCurve2d( dtOCCCurve2dBase const & orig) : dtOCCCurve2d(orig) {
		dt__mustCast(OCCRef().getOCC().Access(), Geom2d_BSplineCurve const, _ptr);
	}
	
	dtOCCBSplineCurve2d::~dtOCCBSplineCurve2d() {
	}

  dtCurve2d * dtOCCBSplineCurve2d::clone( void ) const {
		return new dtOCCBSplineCurve2d( OCCRef() );
	}
	
  int dtOCCBSplineCurve2d::order( void ) const {
    return static_cast<int>(_ptr->Degree());		
	}
	
  int dtOCCBSplineCurve2d::nControlPoints( void ) const {
		return static_cast<int>(_ptr->NbPoles());		
	}
	
  dtPoint2 dtOCCBSplineCurve2d::controlPoint( int const nPoint ) const {
		Standard_Integer nPI = static_cast<Standard_Integer>(nPoint+1);		
		gp_Pnt2d pp = _ptr->Pole(nPI);
		
		return dtPoint2(
						static_cast<float>(pp.Coord(1)), 
						static_cast<float>(pp.Coord(2))
		);		
	}
	
  void dtOCCBSplineCurve2d::setControlPoint( int const nPoint, dtPoint2 const point ) {
		Standard_Integer nPI = static_cast<Standard_Integer>(nPoint+1);		
		gp_Pnt2d pp(
			static_cast<Standard_Real>(point.x()), 
			static_cast<Standard_Real>(point.y())
		);
		
		Handle(Geom2d_Curve) occC = dtOCCCurve2d::OCCRef().getOCC();
		Handle(Geom2d_BSplineCurve) occBsc = Handle(Geom2d_BSplineCurve)::DownCast(occC);
		
		occBsc->SetPole(nPI, pp);
	}
}
