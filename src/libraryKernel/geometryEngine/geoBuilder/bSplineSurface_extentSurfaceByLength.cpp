#include "bSplineSurface_extentSurfaceByLength.h"

#include <logMe/logMe.h>
#include <logMe/dtMacros.h>
#include <geometryEngine/dtOCCSurface.h>
#include "geomSurface_baseConstructOCC.h"

#include <Precision.hxx>
#include <Standard_Failure.hxx>
#include <Standard_ErrorHandler.hxx>
#include <GeomLib.hxx>
#include <Geom_Geometry.hxx>
#include <Geom_BoundedSurface.hxx>


namespace dtOO {
  bSplineSurface_extentSurfaceByLength::bSplineSurface_extentSurfaceByLength(
		dtSurface const * const dtS, 
		int const & parameterDir, float const & length, 
		int const & cont, bool const & forward
	) {
		dt__throwIf(
			!dtOCCSurface::ConstDownCast(dtS), 
			bSplineSurface_extentSurfaceByLength()
		);
		
		//
		// clone surface
		//		
		dtOCCSurface const * const occS = dtOCCSurface::ConstSecureCast(dtS);	
		
		//
		// create input
		//
		Standard_Real const Length = static_cast< Standard_Real >(length);
		Standard_Boolean InU = true;
		if (parameterDir == 1) InU = false;
		Standard_Boolean After = true;
		if (forward == true) After = false;
		Standard_Integer const Cont = static_cast< Standard_Integer >(cont);;
						
		//
		// cast and check surface type
		//
    Handle(Geom_BoundedSurface) bGeo 
		= 
		Handle(Geom_BoundedSurface)::DownCast( occS->OCCRef().getOCC()->Copy() );		
		dt__throwIf(bGeo.IsNull(), bSplineSurface_extentSurfaceByLength());
				
		//
		// do extension
		//
		dt__tryOcc(
      GeomLib::ExtendSurfByLength(bGeo, Length, Cont,	InU, After);,
			<< "Cannot extent surface."
		);

		//
		// create new surface
		//
		dtOCCSurfaceBase base(bGeo);
	  _dtS.reset( geomSurface_baseConstructOCC(base).result() );
	}
	
  bSplineSurface_extentSurfaceByLength::~bSplineSurface_extentSurfaceByLength() {
		
	}
	
  dtSurface * bSplineSurface_extentSurfaceByLength::result( void ) {
		return _dtS->clone();
	}
}
