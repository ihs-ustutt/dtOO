#include "uv_map2dTo3dClosestPointToPoint.h"

#include <interfaceHeaven/staticPropertiesHandler.h>
#include <analyticGeometryHeaven/map2dTo3d.h>
#include <dtAnalysis.h>
#include <Math/Functor.h>
#include <boost/assign.hpp>

namespace dtOO {
	uv_map2dTo3dClosestPointToPoint
    ::uv_map2dTo3dClosestPointToPoint(
      map2dTo3d const * const m2d, dtPoint3 const & pXYZ
	) : _m2d(*m2d), _pXYZ(pXYZ) {
    //
    // init
    //
    _closestUV = ( _m2d % dtPoint2(0.5, 0.5) );
    _distance = dtLinearAlgebra::distance( _pXYZ, _m2d.getPoint( _closestUV ) );
    
    std::vector< float > initGuess = ::boost::assign::list_of(0.0)(0.5)(1.0);
    dt__forAllRefAuto(initGuess, aGuess) {
      // 
      // multidimensional minimization
      //
      dt__pH(dtMinimizer) min(
        dtAnalysis::createMinimizer(":Minuit2::kMigrad:")
      );
      ::ROOT::Math::Functor toMin(
        this, &uv_map2dTo3dClosestPointToPoint::F, 2
      );			
      min->SetFunction(toMin);          

      //
      // set bounds
      //
      min->SetVariable( 0, "U", aGuess, 0.01 );
      min->SetVariableLimits(0, 0., 1.);
      min->SetVariable( 1, "V", aGuess, 0.01 );
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

      if (
        dtLinearAlgebra::distance( 
          _pXYZ, 
          _m2d.getPoint( _m2d % dtPoint2(theRoot[0], theRoot[1]) ) 
        )
        <
        _distance
      ) {
        _closestUV = ( _m2d % dtPoint2(theRoot[0], theRoot[1]) );
        _distance = dtLinearAlgebra::distance( _pXYZ, _m2d.getPoint( _closestUV ) );
      }
    }
	}

	uv_map2dTo3dClosestPointToPoint::~uv_map2dTo3dClosestPointToPoint() {
	}
	
	dtPoint2 uv_map2dTo3dClosestPointToPoint::result( void ) {
		return _closestUV;
	}
  
  float uv_map2dTo3dClosestPointToPoint::distance( void ) {
    return _distance;  
  }
  
	double uv_map2dTo3dClosestPointToPoint::F(
    double const * xx
  ) const {	
    return dtLinearAlgebra::length( 
      _m2d.getPointPercent(xx[0], xx[1]) - _pXYZ 
    );
	}    
}
