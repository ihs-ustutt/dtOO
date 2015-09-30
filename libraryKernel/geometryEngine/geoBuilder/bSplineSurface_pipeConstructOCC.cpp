#include "bSplineSurface_pipeConstructOCC.h"
#include "geometryEngine/dtOCCCurveBase.h"
#include <geometryEngine/dtOCCCurve.h>
#include <geometryEngine/dtOCCSurface.h>
#include <geometryEngine/dtCurve.h>
#include <geometryEngine/dtSurface.h>

#include "geomSurface_baseConstructOCC.h"

#include <Precision.hxx>
#include <Standard_Failure.hxx>
#include <Standard_ErrorHandler.hxx>
#include <GeomLib.hxx>
#include <Geom_Geometry.hxx>
#include <Geom_BoundedSurface.hxx>
#include <GeomFill_Pipe.hxx>

namespace dtOO {
  bSplineSurface_pipeConstructOCC::bSplineSurface_pipeConstructOCC(
    dtCurve const * const aPath, 
    dtCurve const * const aSecOne, 
    dtCurve const * const aSecTwo
  ) {
		dt__throwIf(
			!dtOCCCurve::ConstDownCast(aPath) 
      || 
      !dtOCCCurve::ConstDownCast(aSecOne)
      ||
      !dtOCCCurve::ConstDownCast(aSecTwo)
      , 
			bSplineSurface_pipeConstructOCC()
		);    
    
		//
		// cast curves
		//		
		dtOCCCurve const * const occCPath = dtOCCCurve::ConstSecureCast(aPath);	
    dtOCCCurve const * const occCSecOne = dtOCCCurve::ConstSecureCast(aSecOne);	
    dtOCCCurve const * const occCSecTwo = dtOCCCurve::ConstSecureCast(aSecTwo);	
    
    GeomFill_Pipe aPipe(
      dtOCCCurve::ConstSecureCast(aPath)->OCCRef().getOCC(), 
      dtOCCCurve::ConstSecureCast(aSecOne)->OCCRef().getOCC(),
      dtOCCCurve::ConstSecureCast(aSecTwo)->OCCRef().getOCC()
    );
    aPipe.Perform(
      Precision::Confusion(), Standard_False
    );
    Handle(Geom_Surface) aSurface 
    = 
    Handle(Geom_Surface)::DownCast( aPipe.Surface()->Copy() );
    ;

		//
		// create new surface
		//
		dtOCCSurfaceBase base(aSurface);
	  _dtS.reset( geomSurface_baseConstructOCC(base).result() );
  }

  bSplineSurface_pipeConstructOCC::~bSplineSurface_pipeConstructOCC() {
  }

  dtSurface * bSplineSurface_pipeConstructOCC::result( void ) {
		return _dtS->clone();
	}  
}
