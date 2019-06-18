#include "bSplineSurface_pipeConstructOCC.h"
#include <geometryEngine/dtOCCCurveBase.h>
#include <geometryEngine/dtOCCCurve.h>
#include <geometryEngine/dtOCCSurfaceBase.h>
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
    dtCurve const * const aSecTwo,
    bool const & polynomial,
    int const & absShape,
    int const & maxDegree,
    int const & nbSections          
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
    
    Handle(Geom_Surface) aSurface;
    dt__tryOcc(  
      //
      // create pipe
      //		                
      GeomFill_Pipe aPipe(
        dtOCCCurve::ConstSecureCast(aPath)->OCCRef().getOCC(), 
        dtOCCCurve::ConstSecureCast(aSecOne)->OCCRef().getOCC(),
        dtOCCCurve::ConstSecureCast(aSecTwo)->OCCRef().getOCC()
      );

      aPipe.Perform(
        Precision::Confusion(), 
        polynomial, 
        static_cast<GeomAbs_Shape>(absShape), 
        maxDegree, 
        nbSections
      );
      aSurface 
      = 
      Handle(Geom_Surface)::DownCast( aPipe.Surface()->Copy() );
      ,
      << ""
    );

		//
		// create new surface
		//
		dtOCCSurfaceBase base(aSurface);
	  _dtS.reset( geomSurface_baseConstructOCC(base).result() );
  }
  
  bSplineSurface_pipeConstructOCC::bSplineSurface_pipeConstructOCC(
    dtCurve const * const aPath, 
    dtCurve const * const aSecOne,
    bool const & polynomial,
    int const & absShape,
    int const & maxDegree,
    int const & nbSections
          
  ) {
		dt__throwIf(
			!dtOCCCurve::ConstDownCast(aPath) || !dtOCCCurve::ConstDownCast(aSecOne), 
			bSplineSurface_pipeConstructOCC()
		);    
    
    Handle(Geom_Surface) aSurface;
    
    dt__tryOcc(                
      //
      // create pipe
      //	
      GeomFill_Pipe aPipe(
        dtOCCCurve::ConstSecureCast(aPath)->OCCRef().getOCC(), 
        dtOCCCurve::ConstSecureCast(aSecOne)->OCCRef().getOCC()
      );
    
    
      aPipe.Perform(
        Precision::Confusion(), 
        polynomial, 
        static_cast<GeomAbs_Shape>(absShape), 
        maxDegree, 
        nbSections
      );
      
      aSurface = Handle(Geom_Surface)::DownCast( aPipe.Surface()->Copy() );
      ,
      << ""
    );
    

		//
		// create new surface
		//
		dtOCCSurfaceBase base(aSurface);
	  _dtS.reset( geomSurface_baseConstructOCC(base).result() );
  }  

  bSplineSurface_pipeConstructOCC::bSplineSurface_pipeConstructOCC(
    dtCurve const * const aPath, 
    float const & rr,
    bool const & polynomial,
    int const & absShape,
    int const & maxDegree,
    int const & nbSections          
  ) {
		dt__throwIf(
      !dtOCCCurve::ConstDownCast(aPath), 
			bSplineSurface_pipeConstructOCC()
		);    
    
		//
		// cast curves
		//		
//		dtOCCCurve const * const occCPath = dtOCCCurve::ConstSecureCast(aPath);	
    
    Handle(Geom_Surface) aSurface;
    dt__tryOcc(   
      GeomFill_Pipe aPipe(
        dtOCCCurve::ConstSecureCast(aPath)->OCCRef().getOCC(), 
        static_cast< Standard_Real >(rr)
      );
    
      aPipe.Perform(
        Precision::Confusion(), 
        polynomial, 
        static_cast<GeomAbs_Shape>(absShape), 
        maxDegree, 
        nbSections
      );

      aSurface 
      = 
      Handle(Geom_Surface)::DownCast( aPipe.Surface()->Copy() );
      ,
      << ""
    );

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
