#include "x_vec3dTwoDClosestPointToPoint.h"

#include <interfaceHeaven/staticPropertiesHandler.h>
#include <analyticFunctionHeaven/vec3dTwoD.h>
#include <dtAnalysis.h>
#include <Math/Functor.h>

namespace dtOO {
	x_vec3dTwoDClosestPointToPoint::x_vec3dTwoDClosestPointToPoint(
    vec3dTwoD const * const v3d, aFY const & yy
	) : _v3d(*v3d), _yy(yy) {

    // 
    // multidimensional minimization
    //
    dt__pH(dtMinimizer) min(
      dtAnalysis::createMinimizer(":Minuit2::kMigrad:")
    );
    ::ROOT::Math::Functor toMin(
      this, &x_vec3dTwoDClosestPointToPoint::F, 2
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
    
    _xx = _v3d.x_percent( analyticFunction::aFXTwoD(theRoot[0], theRoot[1]) );
	}

	x_vec3dTwoDClosestPointToPoint::~x_vec3dTwoDClosestPointToPoint() {
	}
	
	aFX x_vec3dTwoDClosestPointToPoint::result( void ) {
		return _yy;
	}
  
	double x_vec3dTwoDClosestPointToPoint::F(
    double const * xx
  ) const {
    return
      dtLinearAlgebra::euclidianNorm( 
        dtLinearAlgebra::subtract(
          _v3d.Y( _v3d % analyticFunction::aFXTwoD(xx[0], xx[1]) ), 
          _yy
        )
      );
	}    
}
