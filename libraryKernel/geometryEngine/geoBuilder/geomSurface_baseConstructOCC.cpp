#include "geomSurface_baseConstructOCC.h"
#include <progHelper.h>

#include <logMe/logMe.h>
#include <geometryEngine/dtOCCSurfaceBase.h>
#include <geometryEngine/dtSurface.h>
#include <geometryEngine/dtOCCSurface.h>
#include <geometryEngine/dtOCCBSplineSurface.h>
#include <geometryEngine/dtOCCBezierSurface.h>
#include <geometryEngine/dtOCCRectangularTrimmedSurface.h>
#include <geometryEngine/dtOCCSurfaceOfRevolution.h>

#include <Precision.hxx>
#include <Standard_Failure.hxx>
#include <Standard_ErrorHandler.hxx>
#include <Standard_TypeDef.hxx>
#include <Geom_Surface.hxx>
#include <Geom_BSplineSurface.hxx>
#include <Geom_BezierSurface.hxx>
#include <Geom_RectangularTrimmedSurface.hxx>
#include <Geom_SurfaceOfRevolution.hxx>

namespace dtOO {
	geomSurface_baseConstructOCC::geomSurface_baseConstructOCC( dtOCCSurfaceBase const & base ) {
		Handle(Geom_Surface) occS = base.getOCC();
		if ( !(Handle(Geom_BSplineSurface)::DownCast(occS).IsNull()) ) {
		  _dtS.reset( new dtOCCBSplineSurface(base) );			
		}
		else if ( !(Handle(Geom_BezierSurface)::DownCast(occS).IsNull()) ) {
		  _dtS.reset( new dtOCCBezierSurface(base) );			
		}
		else if ( !(Handle(Geom_RectangularTrimmedSurface)::DownCast(occS).IsNull()) ) {
		  _dtS.reset( new dtOCCRectangularTrimmedSurface(base) );
		}		
		else if ( !(Handle(Geom_SurfaceOfRevolution)::DownCast(occS).IsNull()) ) {
		  _dtS.reset( new dtOCCSurfaceOfRevolution(base) );			
		}
		else {
			dt__throw(
				geomSurface_baseConstructOCC(), 
				<< dt__eval(occS->DynamicType()->Name()) << std::endl
				<< "Unsupported OCC type."
			);			
		}
	}
	
	geomSurface_baseConstructOCC::~geomSurface_baseConstructOCC() {
	}
	
	dtSurface * geomSurface_baseConstructOCC::result( void ) {
		return _dtS->clone();
	}
}
