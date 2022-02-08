#include "trimmedCurve_twoPointsConnectConstructOCC.h"
#include "progHelper.h"
#include "geometryEngine/dtOCCLine.h"

#include <logMe/logMe.h>
#include <geometryEngine/dtOCCCurveBase.h>
#include <geometryEngine/dtOCCTrimmedCurve.h>

#include <Standard_Failure.hxx>
#include <Standard_ErrorHandler.hxx>
#include <Standard_TypeDef.hxx>
#include <Geom_Line.hxx>
#include <Geom_TrimmedCurve.hxx>
#include <gp_Pnt.hxx>
#include <gp_Dir.hxx>

namespace dtOO {
	trimmedCurve_twoPointsConnectConstructOCC
    ::trimmedCurve_twoPointsConnectConstructOCC( 
      dtPoint3 const & p0, dtPoint3 const & p1 
  ) {
		dtVector3 vv = p1 - p0;
		
		gp_Pnt pp;
		gp_Dir dir;
    dt__tryOcc(		
			pp.SetCoord(
				static_cast< Standard_Real >(p0.x()),
				static_cast< Standard_Real >(p0.y()),
				static_cast< Standard_Real >(p0.z())
			);
			dir.SetCoord(
				static_cast< Standard_Real >(vv.x()),
				static_cast< Standard_Real >(vv.y()),
				static_cast< Standard_Real >(vv.z())
			);		
		,
		<< ""
		);
		//
		// create curve using standard constructor
		//
		Handle(Geom_Line) line 
		= 
		new Geom_Line( pp, dir );

		
		dtOCCCurveBase baseL;
		baseL.setOCC( line );
		ptrHandling< dtCurve > dtL( new dtOCCLine(baseL) );
		Standard_Real U0 = 0.;
		Standard_Real U1 = static_cast<Standard_Real>( dtL->reparam(p1) );
		
		Handle(Geom_TrimmedCurve) curve
		=
		new Geom_TrimmedCurve(line, U0, U1);

		dtOCCCurveBase baseC;
		baseC.setOCC( curve );
		_dtC.reset( new dtOCCTrimmedCurve(baseC) );
	}
	
	trimmedCurve_twoPointsConnectConstructOCC
    ::~trimmedCurve_twoPointsConnectConstructOCC() {
	}
	
	dtCurve * trimmedCurve_twoPointsConnectConstructOCC::result( void ) {
		return _dtC->clone();
	}
}