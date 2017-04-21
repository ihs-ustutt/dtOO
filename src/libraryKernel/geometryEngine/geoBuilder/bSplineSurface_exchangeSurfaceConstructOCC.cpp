#include "bSplineSurface_exchangeSurfaceConstructOCC.h"

#include <logMe/logMe.h>
#include <logMe/dtMacros.h>
#include <geometryEngine/dtOCCBSplineSurface.h>
#include "geomSurface_baseConstructOCC.h"
#include "bSplineSurface_convertOCC.h"

#include <Precision.hxx>
#include <Standard_Failure.hxx>
#include <Standard_ErrorHandler.hxx>
#include <Geom_Geometry.hxx>
#include <Geom_BSplineSurface.hxx>


namespace dtOO {
  bSplineSurface_exchangeSurfaceConstructOCC
    ::bSplineSurface_exchangeSurfaceConstructOCC(
		dtSurface const * const dtS
	) {
    dt__pH(dtSurface) dtBS( bSplineSurface_convertOCC(dtS).result() );
		//
		// cast and check surface type
		//		
    Handle(Geom_BSplineSurface) cc 
		= 
		Handle(Geom_BSplineSurface)::DownCast( 
      dtOCCBSplineSurface::ConstSecureCast(dtBS.get())->OCCRef().getOCC()->Copy() 
    );
    
    //
    // exchange parameter space
    //
    cc->ExchangeUV();

		//
		// create new surface
		//
		dtOCCSurfaceBase base(cc);
	  _dtS.reset( geomSurface_baseConstructOCC(base).result() );
	}
	
  bSplineSurface_exchangeSurfaceConstructOCC
    ::~bSplineSurface_exchangeSurfaceConstructOCC() {
		
	}
	
  dtSurface * bSplineSurface_exchangeSurfaceConstructOCC::result( void ) {
		return _dtS->clone();
	}
}
