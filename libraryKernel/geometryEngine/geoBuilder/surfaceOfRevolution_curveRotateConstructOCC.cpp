#include "surfaceOfRevolution_curveRotateConstructOCC.h"

#include <logMe/logMe.h>
#include <progHelper.h>
#include <geometryEngine/dtOCCCurveBase.h>
#include <geometryEngine/dtOCCCurve.h>
#include <geometryEngine/dtOCCSurfaceOfRevolution.h>

//#include <Precision.hxx>
#include <gp_Pnt.hxx>
#include <gp_Dir.hxx>
#include <gp_Ax1.hxx>
#include <Geom_SurfaceOfRevolution.hxx>


namespace dtOO {
  surfaceOfRevolution_curveRotateConstructOCC::surfaceOfRevolution_curveRotateConstructOCC() {
		
	}
	
  surfaceOfRevolution_curveRotateConstructOCC::surfaceOfRevolution_curveRotateConstructOCC( 
	  dtCurve const & cL, 
    dtPoint3 const & oP, 					
		dtVector3 const & rA
	) {
		dt__ptrAss(dtOCCCurve const * bC, dtOCCCurve::ConstDownCast( &cL ));
		
	  gp_Pnt pp(
		  static_cast<Standard_Real>(oP.x()), 
			static_cast<Standard_Real>(oP.y()),
			static_cast<Standard_Real>(oP.z()) 
		);
		gp_Dir dir(
		  static_cast<Standard_Real>(rA.x()), 
			static_cast<Standard_Real>(rA.y()),
			static_cast<Standard_Real>(rA.z()) 
		);		
	  gp_Ax1 rotAx(pp, dir);
		Handle(Geom_SurfaceOfRevolution) aRes 
		= 
		new Geom_SurfaceOfRevolution( bC->OCCRef().getOCC(), rotAx);

		dtOCCSurfaceBase base;
		base.setOCC(aRes);
		
		_dtS.reset( new dtOCCSurfaceOfRevolution(base) );
	}
	
  surfaceOfRevolution_curveRotateConstructOCC::~surfaceOfRevolution_curveRotateConstructOCC() {
		
	}
	
  dtSurface * surfaceOfRevolution_curveRotateConstructOCC::result( void ) {
		return _dtS->clone();
	}
}
