#include "float_map1dTo3dPointConstCartesian.h"

#include <interfaceHeaven/staticPropertiesHandler.h>
#include <analyticGeometryHeaven/map1dTo3d.h>
#include <dtAnalysis.h>
#include <Math/Functor.h>

namespace dtOO {
	float_map1dTo3dPointConstCartesian::float_map1dTo3dPointConstCartesian(
    map1dTo3d const * const m1d, int const & dir, float const & value
	) : _m1d(*m1d) {
    _dir = dir;
    _value = value;
    
    // 
    // multidimensional minimization
    //
    dt__pH(dtMinimizer) min(
      dtAnalysis::createMinimizer(":Minuit2::kMigrad:")
    );
    ::ROOT::Math::Functor toMin(
      this, &float_map1dTo3dPointConstCartesian::F, 1
    );			
    min->SetFunction(toMin);    
    
    //
    // set bounds
    //
    min->SetVariable( 0, "U", 0.5, 0.01 );
    min->SetVariableLimits(0, 0., 1.);
    
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
    
    _u = _m1d.u_percent( theRoot[0] );
	}

	float_map1dTo3dPointConstCartesian::~float_map1dTo3dPointConstCartesian() {
	}
	
	float float_map1dTo3dPointConstCartesian::result( void ) {
		return _u;
	}

	double float_map1dTo3dPointConstCartesian::F( double const * xx ) const {
    return fabs( _m1d.getPointPercent(xx[0])[_dir] - _value );
	}      
}