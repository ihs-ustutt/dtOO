#include "uvw_map3dTo3dClosestPointToPoint.h"

#include <interfaceHeaven/staticPropertiesHandler.h>
#include <analyticGeometryHeaven/map3dTo3d.h>
#include <dtAnalysis.h>
#include <Math/Functor.h>

namespace dtOO {
	uvw_map3dTo3dClosestPointToPoint
    ::uvw_map3dTo3dClosestPointToPoint(
      map3dTo3d const * const m3d, dtPoint3 const & pXYZ
	) : _m3d(*m3d), _pXYZ(pXYZ) {

    // 
    // multidimensional minimization
    //
    dt__pH(dtMinimizer) min(
      dtAnalysis::createMinimizer(":Minuit2::kMigrad:")
    );
    ::ROOT::Math::Functor toMin(
      this, &uvw_map3dTo3dClosestPointToPoint::F, 3
    );			
    min->SetFunction(toMin);          
    
    //
    // set bounds
    //
    min->SetVariable( 0, "U", 0.5, 0.01 );
    min->SetVariableLimits(0, 0., 1.);
    min->SetVariable( 1, "V", 0.5, 0.01 );
    min->SetVariableLimits(1, 0., 1.);
    min->SetVariable( 2, "W", 0.5, 0.01 );
    min->SetVariableLimits(2, 0., 1.);    

    
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
       
    _closestUVW 
    = 
    _m3d.uvw_percent( dtPoint3(theRoot[0], theRoot[1], theRoot[2]) );
    
    //
    // output
    //
    dtVector3 dist = _pXYZ - _m3d.getPoint(_closestUVW);
    dt__info(
      uvw_map3dTo3dClosestPointToPoint(),
      << logMe::dtFormat(
        "theRoot = (%13.4f, %13.4f, %13.4f)"
      ) % theRoot[0] % theRoot[1] % theRoot[2]
      << std::endl
      << logMe::dtFormat(
        "dist = (%13.4f, %13.4f, %13.4f)"
      ) % dist.x() % dist.y() % dist.z()
      << std::endl      
      << logMe::dtFormat("| dist | = %13.4f") % dtLinearAlgebra::length(dist)
    );
	}

	uvw_map3dTo3dClosestPointToPoint::~uvw_map3dTo3dClosestPointToPoint() {
	}
	
	dtPoint3 uvw_map3dTo3dClosestPointToPoint::result( void ) {
		return _closestUVW;
	}
  
	double uvw_map3dTo3dClosestPointToPoint::F(
    double const * xx
  ) const {	
    return dtLinearAlgebra::length( 
      _m3d.getPointPercent(xx[0], xx[1], xx[2]) - _pXYZ 
    );
	}    
}
