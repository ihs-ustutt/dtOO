#include "bSplineSurface_constrainedFillingConstructOCC.h"

#include <logMe/logMe.h>
#include <geometryEngine/dtCurve.h>
#include <geometryEngine/dtSurface.h>
#include <geometryEngine/dtOCCSurfaceBase.h>
#include <geometryEngine/dtOCCBSplineSurface.h>
#include <geometryEngine/dtOCCGeomFillBoundaryBase.h>
#include <geometryEngine/dtOCCGeomFillBoundary.h>
#include <geometryEngine/geoBuilder/geomFillSimpleBound_curveConstructOCC.h>
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
    dt__vH(dtCurve const *) cV;
    cV.push_back( c0 );    
    cV.push_back( c1 );    
    cV.push_back( c2 );    
    cV.push_back( c3 );    
    dt__pVH(dtCurve) cCV;
    dt__forAllIndex(cV, ii) {
      if ( !dtOCCGeomFillBoundary::Is( cV[ii] ) ) {
        cCV.push_back(
          geomFillSimpleBound_curveConstructOCC( cV[ii] ).result()
        );
      }
      else cCV.push_back(cV[ii]->clone());
    }

		GeomFill_ConstrainedFilling filler(
      static_cast<Standard_Integer>(maxDeg), 
      static_cast<Standard_Integer>(maxSeg)
    );

    dt__tryOcc(    
      filler.Init(
        dtOCCGeomFillBoundary::ConstDownCast(
          &(cCV[0])
        )->OCCBoundaryRef().getOCC(),
        dtOCCGeomFillBoundary::ConstDownCast(
          &(cCV[1])
        )->OCCBoundaryRef().getOCC(),
        dtOCCGeomFillBoundary::ConstDownCast(
          &(cCV[2])
        )->OCCBoundaryRef().getOCC(),
        dtOCCGeomFillBoundary::ConstDownCast(
          &(cCV[3])
        )->OCCBoundaryRef().getOCC()
      );
      ,
      << ""
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