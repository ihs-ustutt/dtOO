#include "geomSurface_surfaceTranslateConstructOCC.h"
#include "geometryEngine/dtOCCBezierSurface.h"
#include "geometryEngine/dtOCCBSplineSurface.h"
#include <geometryEngine/dtOCCSurface.h>
#include <geometryEngine/dtOCCSurfaceBase.h>
#include "geomSurface_baseConstructOCC.h"

#include <Geom_Surface.hxx>
#include <gp_Vec.hxx>

namespace dtOO {
	geomSurface_surfaceTranslateConstructOCC
    ::geomSurface_surfaceTranslateConstructOCC() {
		
	}

	geomSurface_surfaceTranslateConstructOCC
    ::geomSurface_surfaceTranslateConstructOCC(
		dtSurface const * const dtS, dtVector3 const & vv
	) {
		dt__ptrAss( 
      dtOCCSurface const * const ss, dtOCCSurface::ConstDownCast(dtS) 
    );
		
		Geom_Surface const * sP;
		dt__mustCast(ss->OCCRef().getOCC().Access(), Geom_Surface const, sP);
		
		Handle(Geom_Surface) aRes = Handle(Geom_Surface)::DownCast( sP->Copy() );
		aRes->Translate(
      gp_Vec(
        static_cast<Standard_Real>(vv.x()), 
        static_cast<Standard_Real>(vv.y()),
        static_cast<Standard_Real>(vv.z()) 
      )    
    );
    
		//
		// create new surface
		//
	  dtOCCSurfaceBase base(aRes);		
		_dtS.reset( geomSurface_baseConstructOCC(base).result() );
	}

	geomSurface_surfaceTranslateConstructOCC
    ::~geomSurface_surfaceTranslateConstructOCC() {
		
	}
	
	dtSurface * geomSurface_surfaceTranslateConstructOCC::result( void ) {
		return _dtS->clone();		
	}
}
