#include "dtOCCCircle.h"

#include <logMe/logMe.h>
#include "dtOCCCurveBase.h"
#include "progHelper.h"

#include <Standard_TypeDef.hxx>
#include <gp_Pnt.hxx>
#include <Geom_Circle.hxx>
#include <Precision.hxx>

namespace dtOO {
	dtOCCCircle::dtOCCCircle() : dtOCCCurve() {
		_ptr = NULL;		
	}

	dtOCCCircle::dtOCCCircle(dtOCCCurveBase const & orig) : dtOCCCurve(orig) {
		dt__mustCast(OCCRef().getOCC().Access(), Geom_Circle const, _ptr);	
	}

	dtOCCCircle::~dtOCCCircle() {
	}

  dtCurve * dtOCCCircle::clone( void ) const {
		return new dtOCCCircle( OCCRef() );		
	}
	
  int dtOCCCircle::order( void ) const {
		dt__throwUnexpected(order());			
	}
	
  int dtOCCCircle::nControlPoints( void ) const {
		return 0;
	}
	
  dtPoint3 dtOCCCircle::controlPoint( int const nPoint ) const {
		dt__throwUnexpected(controlPoint());		
	}
	
  void dtOCCCircle::setControlPoint( int const nPoint, dtPoint3 const point ) {
		dt__throwUnexpected(setControlPoint());				
	}
	
//  dtCurve * dtOCCCircle::bSplineCurve( void ) const {
//		dtCurve * bS;
//	  if ( !Handle(Geom_Line)::DownCast(_ptr->BasisCurve()).IsNull() )	{
//			std::vector< dtPoint3 > pVec;
//			pVec.push_back( pointPercent(0.) );
//			pVec.push_back( pointPercent(1.) );
//			bS = bSplineCurve_pointConstructOCC(pVec, 1).result();
//		}
//		else if ( !Handle(Geom_BSplineCurve)::DownCast(_ptr->BasisCurve()).IsNull() ) {
//			Handle(Geom_BSplineCurve) bSC = GeomConvert::SplitBSplineCurve(
//        Handle(Geom_BSplineCurve)::DownCast(_ptr->BasisCurve()->Copy()),
//        static_cast<Standard_Real>(_u0), static_cast<Standard_Real>(_u1),
//        Precision::Confusion()							
//			);
//			dtOCCCurveBase cBase;
//			cBase.setOCC( bSC );
//			bS = new dtOCCBSplineCurve(cBase);
//		}
//		else {
//			dt__THROW(
//			  bSplineCurve(),
//				<< dt__eval(Handle(Geom_Line)::DownCast(_ptr->BasisCurve()).IsNull())<< std::endl
//				<< dt__eval(Handle(Geom_BSplineCurve)::DownCast(_ptr->BasisCurve()).IsNull())
//			);
//		}
////		dt__info(bSplineCurve(),
////						<< dt__eval(Handle(Geom_BSplineCurve)::DownCast(_ptr->BasisCurve()).IsNull()) << std::endl
////						<< dt__eval(Handle(Geom_Line)::DownCast(_ptr->BasisCurve()).IsNull()) << std::endl
////						<< dt__eval(_u0) << std::endl
////						<< dt__eval(_u1) << std::endl
////						<< dt__point3d(getPointPercent3d(0.)) << std::endl
////						<< dt__point3d(getPointPercent3d(1.)) << std::endl
////						<< dt__point3d(bS->getPointPercent3d(0.)) << std::endl
////						<< dt__point3d(bS->getPointPercent3d(1.)) << std::endl
////						);
//	  return bS;
//	}	
}
