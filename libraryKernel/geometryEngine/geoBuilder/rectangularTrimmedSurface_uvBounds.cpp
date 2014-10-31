#include "rectangularTrimmedSurface_uvBounds.h"

#include <logMe/logMe.h>
#include <progHelper.h>
//#include <geometryEngine/dtOCCCurveBase.h>
//#include <geometryEngine/dtOCCCurve.h>
#include <geometryEngine/dtOCCRectangularTrimmedSurface.h>

#include <Standard_TypeDef.hxx>
//#include <gp_Pnt.hxx>
//#include <gp_Dir.hxx>
//#include <gp_Ax1.hxx>
//#include <Geom_SurfaceOfRevolution.hxx>
#include <Geom_RectangularTrimmedSurface.hxx>

namespace dtOO {
  rectangularTrimmedSurface_uvBounds::rectangularTrimmedSurface_uvBounds(
	  dtSurface const * dtS, dtPoint2 const & min, dtPoint2 const & max
	) {
    dt__PTRASS(
		  dtOCCSurface const * occS, 
			dtOCCSurface::ConstDownCast(dtS)
		);
		
//		occS->OCCRef().getOCC()
		
		Standard_Real U1 = static_cast<Standard_Real>(min.x());
		Standard_Real U2 = static_cast<Standard_Real>(max.x());
		Standard_Real V1 = static_cast<Standard_Real>(min.y());
		Standard_Real V2 = static_cast<Standard_Real>(max.y());
		
		Handle(Geom_RectangularTrimmedSurface) rts
		=
		new Geom_RectangularTrimmedSurface(
		  occS->OCCRef().getOCC(), U1, U2, V1, V2
		);
		
		dtOCCSurfaceBase base;
		base.setOCC(rts);

		_dtS.reset( new dtOCCRectangularTrimmedSurface(base) );		
	}

  dtSurface * rectangularTrimmedSurface_uvBounds::result( void ) {
		return _dtS->clone();
	}

	rectangularTrimmedSurface_uvBounds::~rectangularTrimmedSurface_uvBounds() {
	}
}
