#include "dtOCCCurve.h"

#include <logMe/logMe.h>
#include "dtOCCCurveBase.h"
#include <progHelper.h>

#include <Standard_Failure.hxx>
#include <Standard_ErrorHandler.hxx>
#include <Standard_TypeDef.hxx>
#include <gp_Pnt.hxx>
#include <gp_Vec.hxx>
#include <Geom_Curve.hxx>
#include <Geom_BoundedCurve.hxx>
#include <GeomAdaptor_Curve.hxx>
#include <GCPnts_AbscissaPoint.hxx>
#include <Extrema_ExtPC.hxx>
#include <Extrema_POnCurv.hxx>
#include <GeomConvert_CompCurveToBSplineCurve.hxx>
#include <Precision.hxx>

namespace dtOO {					
	dtOCCCurve::dtOCCCurve() {
		_curve.reset( new dtOCCCurveBase() );
		_ptr = NULL;
	}

	dtOCCCurve::dtOCCCurve( dtOCCCurveBase const & orig) {
		_curve.reset( new dtOCCCurveBase() );		
		_curve->setOCC( Handle(Geom_Curve)::DownCast(orig.getOCC()->Copy()) );
		dt__mustCast(OCCRef().getOCC().Access(), Geom_Curve const, _ptr);
	}
	
	dtOCCCurve::~dtOCCCurve() {
	}

  float dtOCCCurve::minPara ( int const & dir ) const {
    switch (dir) {
      case 0:
				dt__tryOcc(
          return static_cast<float>(_ptr->FirstParameter());
				,
				  << dt__eval(dir)
				);
        break;
      default:
        dt__throw(minPara(),
              << dt__eval(dir) << std::endl
              << "dir should be 0.");
    }    		    
	}
	
  float dtOCCCurve::maxPara ( int const & dir ) const {
    switch (dir) {
      case 0:
				dt__tryOcc(
          return static_cast<float>(_ptr->LastParameter());
				, 
					<< dt__eval(dir) 
				);
        break;
      default:
        dt__throw(minPara(),
              << dt__eval(dir) << std::endl
              << "dir should be 0.");
    }    		
	}
	
//  int dtOCCCurve::getKind( void ) const {
//		dt__functionNotImplemented(getKind);
//	}
	
  bool dtOCCCurve::closed( void ) const {
		return static_cast<bool>(_ptr->IsClosed());
	}
	
  dtPoint3 dtOCCCurve::point( float const uu ) const {
		Standard_Real uR = static_cast<Standard_Real>(uu);
		gp_Pnt pp;
		dt__tryOcc(
		  pp = _ptr->Value(uR);
		,
		  << dt__eval(uu)
		);
		
		return dtPoint3(
			static_cast<float>(pp.Coord(1)), 
			static_cast<float>(pp.Coord(2)), 
			static_cast<float>(pp.Coord(3))
		);
	}
	
  dtVector3 dtOCCCurve::firstDer( float const uu) const {
		Standard_Real uR = static_cast<Standard_Real>(uu);
		gp_Pnt pp;
		gp_Vec vv;

		dt__tryOcc(
		  _ptr->D1(uR, pp, vv);
		,
		  << dt__eval(uu)
		);
			
		return dtVector3(
						static_cast<float>(vv.Coord(1)), 
						static_cast<float>(vv.Coord(2)), 
						static_cast<float>(vv.Coord(3))
		);		
	}
		
	/**
   * @todo Make tolerance adjustable or better automatically adjustable.
   */
  float dtOCCCurve::l_u( float const uu ) const {
    GeomAdaptor_Curve gac;
		gac.Load( _curve->getOCC() );
		
		Standard_Real uuR = static_cast<Standard_Real>(uu);
		Standard_Real ll;
		dt__tryOcc(
		  ll 
			= 
			GCPnts_AbscissaPoint::Length( 
				gac, 
				_ptr->FirstParameter(),
				uuR,
				Precision::Confusion()
			);
			,
			<< dt__eval(uu)
		);
				
		
		return static_cast<float>(ll);
	}

	/**
   * @todo Make tolerance adjustable or better automatically adjustable.
   */	
  float dtOCCCurve::u_l( float const length ) const {
    GeomAdaptor_Curve gac;
		gac.Load( _curve->getOCC() );
		
		Standard_Real llR = static_cast<Standard_Real>(length);
		Standard_Real uu;
	  Standard_Real uI = getUMin() + .5 * (getUMax()-getUMin());
		dt__tryOcc(
		  GCPnts_AbscissaPoint ap(
				gac, 
				llR, 
				_ptr->FirstParameter(), uI, 
				Precision::Confusion()
			);
		  uu = ap.Parameter();
			,
			<< dt__eval(length) << std::endl
			<< dt__eval(uu)
		);
		
		return static_cast<float>(uu);						
	}
	
	float dtOCCCurve::reparam(dtPoint3 const point) const {
		gp_Pnt pp(
		  static_cast<Standard_Real>(point.x()),
			static_cast<Standard_Real>(point.y()),
			static_cast<Standard_Real>(point.z()) 
	  );
		GeomAdaptor_Curve gac;
		gac.Load( _curve->getOCC() );
		
		Extrema_ExtPC ext(pp, gac);
		
		Extrema_POnCurv epp = ext.Point(1); 
		
		return static_cast<float>( epp.Parameter() );
	}
	
	void dtOCCCurve::revert( void ) {
		Handle(Geom_Curve) rev = _ptr->Reversed();
		_curve->setOCC(rev);
		dt__mustCast(OCCRef().getOCC().Access(), Geom_Curve const, _ptr);		
	}	
	
	void dtOCCCurve::translate( dtVector3 const & tt ) {
		gp_Vec vv(
		  static_cast<Standard_Real>(tt.x()),
			static_cast<Standard_Real>(tt.y()),
			static_cast<Standard_Real>(tt.z()) 
		);
		OCCRef().getOCC()->Translate(vv);
	}
	
	dtOCCCurveBase const & dtOCCCurve::OCCRef( void ) const {
		return *(_curve.get());
	}
	dtOCCCurveBase & dtOCCCurve::OCCRef( void ) {
		return *(_curve.get());
	}	
}
