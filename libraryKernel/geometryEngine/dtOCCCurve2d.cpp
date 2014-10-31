#include "dtOCCCurve2d.h"

#include <logMe/logMe.h>
#include "dtOCCCurve2dBase.h"
#include <progHelper.h>

#include <Standard_TypeDef.hxx>
#include <gp_Pnt2d.hxx>
#include <gp_Vec2d.hxx>
#include <Geom2d_Curve.hxx>
#include <Geom2d_BoundedCurve.hxx>
#include <Geom2dAdaptor_Curve.hxx>
#include <GCPnts_AbscissaPoint.hxx>
#include <Extrema_ExtPC2d.hxx>
#include <Extrema_POnCurv2d.hxx>

namespace dtOO {					
	dtOCCCurve2d::dtOCCCurve2d() {
		_curve.reset( new dtOCCCurve2dBase() );
		_ptr = NULL;
	}

	dtOCCCurve2d::dtOCCCurve2d( dtOCCCurve2dBase const & orig) {
		_curve.reset( new dtOCCCurve2dBase() );		
		_curve->setOCC( Handle(Geom2d_Curve)::DownCast(orig.getOCC()->Copy()) );
		dt__MUSTDOWNCAST(OCCRef().getOCC().Access(), Geom2d_Curve const, _ptr);
	}
	
	dtOCCCurve2d::~dtOCCCurve2d() {
	}

  float dtOCCCurve2d::minPara ( int const & dir ) const {
    switch (dir) {
      case 0:
        return static_cast<float>(_ptr->FirstParameter());
        break;
      default:
        dt__THROW(getMin(),
              << DTLOGEVAL(dir) << LOGDEL
              << "dir should be 0.");
    }    		    
	}
	
  float dtOCCCurve2d::maxPara ( int const & dir ) const {
    switch (dir) {
      case 0:
        return static_cast<float>(_ptr->LastParameter());
        break;
      default:
        dt__THROW(getMin(),
              << DTLOGEVAL(dir) << LOGDEL
              << "dir should be 0.");
    }    		
	}
	
//  int dtOCCCurve2d::kind( void ) const {
//		DTFUNCTIONNOTI(kind);
//	}
	
  bool dtOCCCurve2d::closed( void ) const {
		return static_cast<bool>(_ptr->IsClosed());
	}
	
  dtPoint2 dtOCCCurve2d::point( float const uu ) const {
		Standard_Real uR = static_cast<Standard_Real>(uu);
		gp_Pnt2d pp = _ptr->Value(uR);
		
		return dtPoint2(
						static_cast<float>(pp.Coord(1)), 
						static_cast<float>(pp.Coord(2))
		);
	}
	
  dtVector2 dtOCCCurve2d::firstDer( float const uu) const {
		Standard_Real uR = static_cast<Standard_Real>(uu);
		gp_Pnt2d pp;
		gp_Vec2d vv;
		
		_ptr->D1(uR, pp, vv);
		
		return dtVector2(
						static_cast<float>(vv.Coord(1)), 
						static_cast<float>(vv.Coord(2))
		);		
	}
	
  dtVector2 dtOCCCurve2d::normal( float const uu) const {
//		Standard_Real uR = static_cast<Standard_Real>(uu);
//		gp_Pnt2d pp;
//		gp_Vec2d vv;
//		
//		_ptr->D1(uR, pp, vv);
//		
//		return dtVector2(
//						static_cast<float>(vv.Coord(1)), 
//						static_cast<float>(vv.Coord(2))
//		);		
		DTFUNCTIONNOTI(normal);
	}	
		
  float dtOCCCurve2d::l_u( float const uu ) const {
    Geom2dAdaptor_Curve gac;
		gac.Load( _curve->getOCC() );
		
		Standard_Real uuR = static_cast<Standard_Real>(uu);
		Standard_Real ll 
	  = 
		GCPnts_AbscissaPoint::Length( 
		  gac, 
			_ptr->FirstParameter(),
			uuR
		);
		
		return static_cast<float>(ll);
	}
	
  float dtOCCCurve2d::u_l( float const length ) const {
    Geom2dAdaptor_Curve gac;
		gac.Load( _curve->getOCC() );
		
		Standard_Real llR = static_cast<Standard_Real>(length);
		GCPnts_AbscissaPoint ap(gac, llR, _ptr->FirstParameter());
		
		Standard_Real uu = ap.Parameter();
		
		return static_cast<float>(uu);						
	}

	float dtOCCCurve2d::reparam(dtPoint2 const point) const {
		gp_Pnt2d pp(
		  static_cast<Standard_Real>(point.x()),
			static_cast<Standard_Real>(point.y())
	  );
		Geom2dAdaptor_Curve gac;
		gac.Load( _curve->getOCC() );
		
		Extrema_ExtPC2d ext(pp, gac);
		
		Extrema_POnCurv2d epp = ext.Point(1); 
		
		return static_cast<float>( epp.Parameter() );
	}	
	
	void dtOCCCurve2d::revert( void ) {
		Handle(Geom2d_Curve) rev = _ptr->Reversed();
		_curve->setOCC(rev);
		dt__MUSTDOWNCAST(OCCRef().getOCC().Access(), Geom2d_Curve const, _ptr);		
	}	
	
	dtOCCCurve2dBase const & dtOCCCurve2d::OCCRef( void ) const {
		return *(_curve.get());
	}
}
