#include "bSplineSurface_constrainedFillingConstructOCC.h"


#include <logMe/logMe.h>
#include <geometryEngine/dtCurve.h>
#include <geometryEngine/dtSurface.h>
#include <geometryEngine/dtOCCSurfaceBase.h>
#include <geometryEngine/dtOCCBSplineSurface.h>
#include <geometryEngine/dtOCCGeomFillBoundaryBase.h>
#include <geometryEngine/dtOCCGeomFillBoundary.h>
#include <Standard_Failure.hxx>
#include <Standard_ErrorHandler.hxx>
#include <Standard_TypeDef.hxx>
#include <GeomFill_ConstrainedFilling.hxx>
#include <GeomFill_Boundary.hxx>
#include <Geom_BSplineSurface.hxx>
#include <Precision.hxx>

namespace dtOO {	
	bSplineSurface_constrainedFillingConstructOCC
    ::bSplineSurface_constrainedFillingConstructOCC( 
		dtCurve const * c0, dtCurve const * c1, 
		dtCurve const * c2, dtCurve const * c3,
    int const & maxDeg, int const & maxSeg
	) {
		GeomFill_ConstrainedFilling filler(
      static_cast<Standard_Integer>(maxDeg), 
      static_cast<Standard_Integer>(maxSeg)
    );
    filler.Init(
      dtOCCGeomFillBoundary::ConstDownCast(c0)->OCCBoundaryRef().getOCC(),
      dtOCCGeomFillBoundary::ConstDownCast(c1)->OCCBoundaryRef().getOCC(),
      dtOCCGeomFillBoundary::ConstDownCast(c2)->OCCBoundaryRef().getOCC(),
      dtOCCGeomFillBoundary::ConstDownCast(c3)->OCCBoundaryRef().getOCC()
    );
    
    _dtS.reset(
      new dtOCCBSplineSurface( 
        dtOCCSurfaceBase(
          Handle(Geom_Surface)::DownCast(filler.Surface()->Copy()) 
        )
      )
    );
	}
	
	bSplineSurface_constrainedFillingConstructOCC
    ::~bSplineSurface_constrainedFillingConstructOCC() {
	}
	
	dtSurface * bSplineSurface_constrainedFillingConstructOCC::result( void ) {
		return _dtS->clone();
	}
}