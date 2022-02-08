#include "u_geomCurveClosestPoint.h"

#include <interfaceHeaven/staticPropertiesHandler.h>
#include <geometryEngine/dtCurve.h>
#include <dtAnalysis.h>
#include <Math/Functor.h>

namespace dtOO {
	u_geomCurveClosestPoint
    ::u_geomCurveClosestPoint(
      dtCurve const * const dtC, dtPoint3 const & pXYZ
	) : _dtC(*dtC), _pXYZ(pXYZ) {

    // 
    // multidimensional minimization
    //
    dt__pH(dtMinimizer) min(
      dtAnalysis::createMinimizer(":Minuit2::kMigrad:")
    );
    ::ROOT::Math::Functor toMin(
      this, &u_geomCurveClosestPoint::F, 1
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
    
    _closestU = _dtC.u_uPercent( theRoot[0] );
	}

	u_geomCurveClosestPoint::~u_geomCurveClosestPoint() {
	}
	
	dtReal u_geomCurveClosestPoint::result( void ) {
		return _closestU;
	}
  
	double u_geomCurveClosestPoint::F(
    double const * xx
  ) const {	
    return dtLinearAlgebra::length( 
      _dtC.pointPercent(xx[0]) - _pXYZ 
    );
	}    
}
