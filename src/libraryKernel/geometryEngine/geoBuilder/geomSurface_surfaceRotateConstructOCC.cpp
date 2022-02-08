#include "geomSurface_surfaceRotateConstructOCC.h"
#include "geometryEngine/dtOCCBezierSurface.h"
#include "geometryEngine/dtOCCBSplineSurface.h"
#include <geometryEngine/dtOCCSurface.h>
#include <geometryEngine/dtOCCSurfaceBase.h>
#include "geomSurface_baseConstructOCC.h"

#include <Geom_Surface.hxx>
#include <gp_Pnt.hxx>
#include <gp_Dir.hxx>
#include <gp_Ax1.hxx>
#include <Geom_BSplineSurface.hxx>
#include <Geom_BezierSurface.hxx>

namespace dtOO {
	geomSurface_surfaceRotateConstructOCC::geomSurface_surfaceRotateConstructOCC() {
		
	}

	geomSurface_surfaceRotateConstructOCC::geomSurface_surfaceRotateConstructOCC(
		dtSurface const * const dtS, 
		dtPoint3 const & oP, 
		dtVector3 const & rA, 
		dtReal const & angle
	) {
		dt__ptrAss( 
		  dtOCCSurface const * const ss, dtOCCSurface::ConstDownCast(dtS) 
    );
		
		Geom_Surface const * sP;
		dt__mustCast(ss->OCCRef().getOCC().get(), Geom_Surface const, sP);
		
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
		Standard_Real angleR = static_cast< Standard_Real >(angle);
		
		Handle(Geom_Surface) aRes = Handle(Geom_Surface)::DownCast( sP->Copy() );
		aRes->Rotate(rotAx, angleR);
    
		//
		// create new surface
		//
	  dtOCCSurfaceBase base(aRes);		
		_dtS.reset( geomSurface_baseConstructOCC(base).result() );
	}

	geomSurface_surfaceRotateConstructOCC::~geomSurface_surfaceRotateConstructOCC() {
		
	}
	
	dtSurface * geomSurface_surfaceRotateConstructOCC::result( void ) {
		return _dtS->clone();		
	}
}
