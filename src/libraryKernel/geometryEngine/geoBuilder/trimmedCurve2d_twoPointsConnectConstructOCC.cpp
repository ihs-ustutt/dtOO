#include "trimmedCurve2d_twoPointsConnectConstructOCC.h"
#include "progHelper.h"
#include <logMe/logMe.h>
#include <geometryEngine/dtOCCLine2d.h>
#include <geometryEngine/dtOCCCurve2dBase.h>
#include <geometryEngine/dtOCCTrimmedCurve2d.h>

#include <Standard_Failure.hxx>
#include <Geom2d_Line.hxx>
#include <Geom2d_TrimmedCurve.hxx>
#include <gp_Pnt2d.hxx>
#include <gp_Dir2d.hxx>

namespace dtOO {
	trimmedCurve2d_twoPointsConnectConstructOCC::trimmedCurve2d_twoPointsConnectConstructOCC( dtPoint2 const & p0, dtPoint2 const & p1 ) {
		gp_Pnt2d pp(
			static_cast< Standard_Real >(p0.x()),
			static_cast< Standard_Real >(p0.y())
		);
		dtVector2 vv = p1 - p0;
		gp_Dir2d dir(
			static_cast< Standard_Real >(vv.x()),
			static_cast< Standard_Real >(vv.y())
		);		
		//
		// create curve using standard constructor
		//
		Handle(Geom2d_Line) line 
		= 
		new Geom2d_Line( pp, dir );

		
		dtOCCCurve2dBase baseL;
		baseL.setOCC( line );
		ptrHandling< dtCurve2d > dtL( new dtOCCLine2d(baseL) );
		Standard_Real U0 = 0.;
		Standard_Real U1 = static_cast<Standard_Real>( dtL->reparam(p1) );
		
		Handle(Geom2d_TrimmedCurve) curve
		=
		new Geom2d_TrimmedCurve(line, U0, U1);

		dtOCCCurve2dBase baseC;
		baseC.setOCC( curve );
		_dtC2d.reset( 
		  new dtOCCTrimmedCurve2d(
		    baseC, static_cast<float>(U0), static_cast<float>(U1)
		  ) 
		);
		
//		dt__info(
//		  trimmedCurve2d_twoPointsConnectConstructOCC(),
//			<< dt__eval(p0) << std::endl
//			<< dt__eval(p1) << std::endl
//			<< dt__point2d(_dtC2d->pointPercent(0.)) << std::endl
//			<< dt__point2d(_dtC2d->pointPercent(1.))
//		);
	}
	
	trimmedCurve2d_twoPointsConnectConstructOCC::~trimmedCurve2d_twoPointsConnectConstructOCC() {
	}
	
	dtCurve2d * trimmedCurve2d_twoPointsConnectConstructOCC::result( void ) {
		return _dtC2d->clone();
	}
}