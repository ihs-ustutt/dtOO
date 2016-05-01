#include "geomSurface_geomSurfaceTranslateConstructOCC.h"
#include <geometryEngine/dtOCCSurface.h>
#include "geomSurface_baseConstructOCC.h"

#include <logMe/logMe.h>
#include <geometryEngine/dtSurface.h>
#include <geometryEngine/dtOCCSurfaceBase.h>
#include <Geom_Surface.hxx>
#include <gp_Vec.hxx>

namespace dtOO {
  geomSurface_geomSurfaceTranslateConstructOCC::geomSurface_geomSurfaceTranslateConstructOCC( dtSurface const * const ss, dtVector3 const & vv ) {
		//
		// cast and clone existing surface
		//
		dtOCCSurface const * const occS = dtOCCSurface::ConstDownCast(ss);
		Handle(Geom_Surface) nS 
		= 
		Handle(Geom_Surface)::DownCast(occS->OCCRef().getOCC()->Copy());
		
		//
		// do translation
		//
		gp_Vec gV(vv.x(), vv.y(), vv.z());
		nS->Translate(gV);
		
		//
		// create new surface
		//
	  dtOCCSurfaceBase base(nS);		
		_dtS.reset( geomSurface_baseConstructOCC(base).result() );
	}
	
	geomSurface_geomSurfaceTranslateConstructOCC::~geomSurface_geomSurfaceTranslateConstructOCC() {
	}
	
	dtSurface * geomSurface_geomSurfaceTranslateConstructOCC::result( void ) {
		return _dtS->clone();
	}
}