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
		dt__mustCast(OCCRef().getOCC().Access(), Geom_BSplineCurve const, _ptr);
	}
	
	dtOCCBSplineCurve::~dtOCCBSplineCurve() {
	}

  dtCurve * dtOCCBSplineCurve::clone( void ) const {
		return new dtOCCBSplineCurve( OCCRef() );
	}
	
  int dtOCCBSplineCurve::order( void ) const {
    return static_cast<int>(_ptr->Degree());		
	}
	
  int dtOCCBSplineCurve::nControlPoints( void ) const {
		return static_cast<int>(_ptr->NbPoles());		
	}
	
  dtPoint3 dtOCCBSplineCurve::controlPoint( int const nPoint ) const {
		Standard_Integer nPI = static_cast<Standard_Integer>(nPoint+1);		
		gp_Pnt pp = _ptr->Pole(nPI);
		
		return dtPoint3(
						static_cast<float>(pp.Coord(1)), 
						static_cast<float>(pp.Coord(2)), 
						static_cast<float>(pp.Coord(3))
		);		
	}
	
  void dtOCCBSplineCurve::setControlPoint( int const nPoint, dtPoint3 const point ) {
		Standard_Integer nPI = static_cast<Standard_Integer>(nPoint+1);		
		gp_Pnt pp(
			static_cast<Standard_Real>(point.x()), 
			static_cast<Standard_Real>(point.y()),
			static_cast<Standard_Real>(point.z())
		);
		
		Handle(Geom_Curve) occC = dtOCCCurve::OCCRef().getOCC();
		Handle(Geom_BSplineCurve) occBsc = Handle(Geom_BSplineCurve)::DownCast(occC);
		
		occBsc->SetPole(nPI, pp);
	}

	std::string dtOCCBSplineCurve::dumpToString( void ) const {
		std::stringstream ss;
		
		ss 
    << 
    dt__dumpToString(
      << dtOCCCurve::dumpToString() << std::endl
      << dt__eval(_ptr->FirstParameter()) << std::endl
      << dt__eval(_ptr->LastParameter()) << std::endl
      << dt__eval(_ptr->IsRational()) << std::endl
      << dt__eval(_ptr->IsPeriodic()) << std::endl
      << dt__eval(_ptr->Continuity()) << std::endl
      << "GeomAbs_C0=0, GeomAbs_G1, GeomAbs_C1, GeomAbs_G2," << std::endl
      << "GeomAbs_C2, GeomAbs_C3, GeomAbs_CN"
    );
    
		
		return ss.str();
	}    
}