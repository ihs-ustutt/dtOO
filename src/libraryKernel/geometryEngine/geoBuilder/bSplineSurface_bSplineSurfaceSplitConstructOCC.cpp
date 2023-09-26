#include "bSplineSurface_bSplineSurfaceSplitConstructOCC.h"

#include <progHelper.h>
#include <logMe/logMe.h>

#include <geometryEngine/dtOCCSurfaceBase.h>
#include <geometryEngine/dtOCCBSplineSurface.h>

#include <Standard_Failure.hxx>
#include <Standard_ErrorHandler.hxx>
#include <Standard_TypeDef.hxx>
#include <Precision.hxx>
#include <Geom_BSplineSurface.hxx>
#include <GeomConvert.hxx>

namespace dtOO {
  bSplineSurface_bSplineSurfaceSplitConstructOCC
    ::bSplineSurface_bSplineSurfaceSplitConstructOCC( 
    dtSurface const * const dtC, 
    dtInt const & dir, dtReal const & p0, dtReal const & p1
  ) {
    dt__ptrAss(
      dtOCCBSplineSurface const * const bS,
			dtOCCBSplineSurface::ConstDownCast(dtC)
		);
    
    Handle(Geom_BSplineSurface) geomBSpline 
    = 
    Handle(Geom_BSplineSurface)::DownCast( bS->OCCRef().getOCC() );
    
    dt__throwIf(
      geomBSpline.IsNull(), bSplineSurface_bSplineSurfaceSplitConstructOCC()
    );

    bool USplit = true;
    if ( dir == 0 ) {
      USplit = true;
    }
    else if (dir == 1) {
      USplit = false;
    }
    else dt__throwUnexpected(bSplineSurface_bSplineSurfaceSplitConstructOCC());

		dtOCCSurfaceBase base;

    dt__tryOcc(        
      base.setOCC( 
        GeomConvert::SplitBSplineSurface( 
          geomBSpline, 
          Standard_Real(p0), Standard_Real(p1), Standard_Boolean(USplit), 
          Precision::Confusion()
        )
      );
      ,
      << "Split fails."
    );
		_dtS.reset( new dtOCCBSplineSurface(base) );
  } 
  
	bSplineSurface_bSplineSurfaceSplitConstructOCC
    ::~bSplineSurface_bSplineSurfaceSplitConstructOCC() {
	}
	
	dtSurface * bSplineSurface_bSplineSurfaceSplitConstructOCC::result( void ) {
		return _dtS->clone();
	}
}
