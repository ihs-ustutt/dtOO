#include "dtOCCTrimmedCurve.h"

#include <logMe/logMe.h>
#include "dtOCCCurveBase.h"
#include "dtOCCBSplineCurve.h"
#include "progHelper.h"

#include <Standard_TypeDef.hxx>
#include <gp_Pnt.hxx>
#include <Geom_TrimmedCurve.hxx>
#include <Geom_BSplineCurve.hxx>
#include <Geom_Line.hxx>
#include <Precision.hxx>
#include <GeomAdaptor_Curve.hxx>
#include <GeomAdaptor_HCurve.hxx>
#include <GeomConvert.hxx>
#include <geometryEngine/geoBuilder/bSplineCurve_pointConstructOCC.h>

namespace dtOO {
	dtOCCTrimmedCurve::dtOCCTrimmedCurve() : dtOCCCurve() {
		_ptr = NULL;		
	}

	dtOCCTrimmedCurve::dtOCCTrimmedCurve(
    dtOCCCurveBase const & orig, float const u0, float const u1
  ) : dtOCCCurve(orig) {
		dt__mustCast(OCCRef().getOCC().Access(), Geom_TrimmedCurve const, _ptr);
		_u0 = u0;
		_u1 = u1;
	}

	dtOCCTrimmedCurve::~dtOCCTrimmedCurve() {
	}

  dtCurve * dtOCCTrimmedCurve::clone( void ) const {
		return new dtOCCTrimmedCurve( OCCRef(), _u0, _u1 );		
	}
	
  int dtOCCTrimmedCurve::order( void ) const {
		dt__throwUnexpected(order());			
	}
	
  int dtOCCTrimmedCurve::nControlPoints( void ) const {
		return 0;			
	}
	
  dtPoint3 dtOCCTrimmedCurve::controlPoint( int const nPoint ) const {
		dt__throwUnexpected(controlPoint());		
	}
	
  void dtOCCTrimmedCurve::setControlPoint( 
    int const nPoint, dtPoint3 const point 
  ) {
		dt__throwUnexpected(setControlPoint());				
	}
	
  dtCurve * dtOCCTrimmedCurve::bSplineCurve( void ) const {
		dtCurve * bS;
	  if ( !Handle(Geom_Line)::DownCast(_ptr->BasisCurve()).IsNull() )	{
			std::vector< dtPoint3 > pVec;
			pVec.push_back( pointPercent(0.) );
			pVec.push_back( pointPercent(1.) );
			bS = bSplineCurve_pointConstructOCC(pVec, 1).result();
		}
		else if ( 
      !Handle(Geom_BSplineCurve)::DownCast(_ptr->BasisCurve()).IsNull() 
    ) {
			Handle(Geom_BSplineCurve) bSC = GeomConvert::SplitBSplineCurve(
        Handle(Geom_BSplineCurve)::DownCast(_ptr->BasisCurve()->Copy()),
        static_cast<Standard_Real>(_u0), static_cast<Standard_Real>(_u1),
        Precision::Confusion()							
			);
			dtOCCCurveBase cBase;
			cBase.setOCC( bSC );
			bS = new dtOCCBSplineCurve(cBase);
		}
		else {
			dt__throw(
			  bSplineCurve(),
				<< dt__eval(
          Handle(Geom_Line)::DownCast(_ptr->BasisCurve()).IsNull()
        ) << std::endl
				<< dt__eval(
          Handle(Geom_BSplineCurve)::DownCast(_ptr->BasisCurve()).IsNull()
        )
			);
		}
//		dt__info(bSplineCurve(),
//						<< dt__eval(Handle(Geom_BSplineCurve)::DownCast(_ptr->BasisCurve()).IsNull()) << std::endl
//						<< dt__eval(Handle(Geom_Line)::DownCast(_ptr->BasisCurve()).IsNull()) << std::endl
//						<< dt__eval(_u0) << std::endl
//						<< dt__eval(_u1) << std::endl
//						<< dt__point3d(getPointPercent3d(0.)) << std::endl
//						<< dt__point3d(getPointPercent3d(1.)) << std::endl
//						<< dt__point3d(bS->getPointPercent3d(0.)) << std::endl
//						<< dt__point3d(bS->getPointPercent3d(1.)) << std::endl
//						);
	  return bS;
	}	
}