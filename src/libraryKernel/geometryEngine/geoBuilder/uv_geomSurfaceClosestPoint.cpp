#include "uv_geomSurfaceClosestPoint.h"

#include <interfaceHeaven/staticPropertiesHandler.h>
#include <geometryEngine/dtSurface.h>
#include <dtAnalysis.h>
#include <Math/Functor.h>

namespace dtOO {
	uv_geomSurfaceClosestPoint
    ::uv_geomSurfaceClosestPoint(
      dtSurface const * const dtS, dtPoint3 const & pXYZ
	) : _dtS(*dtS), _pXYZ(pXYZ) {

    // 
    // multidimensional minimization
    //
    dt__pH(dtMinimizer) min(
      dtAnalysis::createMinimizer(":Minuit2::kMigrad:")
    );
    ::ROOT::Math::Functor toMin(
      this, &uv_geomSurfaceClosestPoint::F, 2
    );			
    min->SetFunction(toMin);          
    
    //
    // set bounds
    //
    min->SetVariable( 0, "U", 0.5, 0.01 );
    min->SetVariableLimits(0, 0., 1.);
    min->SetVariable( 1, "V", 0.5, 0.01 );
    min->SetVariableLimits(1, 0., 1.);

    
    //
    // minimizer options
    //        
    min->SetMaxFunctionCalls(1000000);
    min->SetMaxIterations(1000);
    min->SetTolerance( 1.e-8 );			
    min->SetPrintLevel(
      staticPropertiesHandler::getInstance()->getOptionInt("root_printLevel") 
    );
    
    //
    // minimize
    //
    min->Minimize();
    double const * const theRoot = min->X();
    
    _closestUV = _dtS.uv_uvPercent( dtPoint2(theRoot[0], theRoot[1]) );
	}

	uv_geomSurfaceClosestPoint::~uv_geomSurfaceClosestPoint() {
	}
	
	dtPoint2 uv_geomSurfaceClosestPoint::result( void ) {
		return _closestUV;
	}
  
	double uv_geomSurfaceClosestPoint::F(
    double const * xx
  ) const {	
    return dtLinearAlgebra::length( 
      _dtS.getPointPercent3d(xx[0], xx[1]) - _pXYZ 
    );
	}    
}