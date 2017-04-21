#include "bSplineSurface_convertOCC.h"

#include <logMe/logMe.h>
#include <logMe/dtMacros.h>

#include <geometryEngine/dtOCCSurface.h>
#include <geometryEngine/dtOCCBSplineSurface.h>
#include "geomSurface_baseConstructOCC.h"

#include <Precision.hxx>
#include <Standard_Failure.hxx>
#include <Standard_ErrorHandler.hxx>
#include <GeomConvert.hxx>
#include <Geom_BSplineSurface.hxx>
#include <Geom_Surface.hxx>


namespace dtOO {
  bSplineSurface_convertOCC::bSplineSurface_convertOCC(
		dtSurface const * const dtS
	) {
    //
    // already a BSplineSurface
    //
    if ( dtOCCBSplineSurface::Is(dtS) ) {
      _dtS.reset( dtS->clone() );
      return;
    }
    
		//
		// cast and check surface type
		//		
    Handle(Geom_Surface) ss 
		= 
		Handle(Geom_Surface)::DownCast( 
      dtOCCSurface::MustConstDownCast(dtS)->OCCRef().getOCC()->Copy() 
    );
    Handle(Geom_BSplineSurface) bS;
    dt__tryOcc(
      bS = GeomConvert::SurfaceToBSplineSurface( ss );
      ,
      << ""
    );

		//
		// create new surface
		//
		dtOCCSurfaceBase base(bS);
	  _dtS.reset( geomSurface_baseConstructOCC(base).result() );
	}
	
  bSplineSurface_convertOCC::~bSplineSurface_convertOCC() {
		
	}
	
  dtSurface * bSplineSurface_convertOCC::result( void ) {
		return _dtS->clone();
	}
}
