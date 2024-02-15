/*---------------------------------------------------------------------------*\
  dtOO < design tool Object-Oriented >
    
    Copyright (C) 2024 A. Tismer.
-------------------------------------------------------------------------------
License
    This file is part of dtOO.

    dtOO is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the LICENSE.txt file in the
    dtOO root directory for more details.

    You should have received a copy of the License along with dtOO.

\*---------------------------------------------------------------------------*/

#include "pairUUV_map1dTo3dClosestPointToMap2dTo3d.h"

#include <interfaceHeaven/staticPropertiesHandler.h>
#include <analyticGeometryHeaven/map1dTo3d.h>
#include <analyticGeometryHeaven/map2dTo3d.h>
#include <dtAnalysis.h>
#include <Math/Functor.h>

namespace dtOO {
	pairUUV_map1dTo3dClosestPointToMap2dTo3d
    ::pairUUV_map1dTo3dClosestPointToMap2dTo3d(
      map1dTo3d const * const m1d, map2dTo3d const * const m2d
	) : _m1d(*m1d), _m2d(*m2d) {

    // 
    // multidimensional minimization
    //
    dt__pH(dtMinimizer) min(
      dtAnalysis::createMinimizer(":Minuit2::kMigrad:")
    );
    ::ROOT::Math::Functor toMin(
      this, &pairUUV_map1dTo3dClosestPointToMap2dTo3d::F, 3
    );			
    min->SetFunction(toMin);          
    
    //
    // set bounds
    //
    min->SetVariable( 0, "U_0", 0.5, 0.01 );
    min->SetVariableLimits(0, 0., 1.);
    min->SetVariable( 1, "U_1", 0.5, 0.01 );
    min->SetVariableLimits(1, 0., 1.);
    min->SetVariable( 2, "V_1", 0.5, 0.01 );
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
    
    _closestUUV.first = ( _m1d % theRoot[0] );
    _closestUUV.second = ( _m2d % dtPoint2( theRoot[1], theRoot[2] ) );
	}

	pairUUV_map1dTo3dClosestPointToMap2dTo3d
    ::~pairUUV_map1dTo3dClosestPointToMap2dTo3d() {
	}
	
	std::pair< dtReal, dtPoint2 > pairUUV_map1dTo3dClosestPointToMap2dTo3d
    ::result( void ) {
		return _closestUUV;
	}
  
	double pairUUV_map1dTo3dClosestPointToMap2dTo3d::F(
    double const * xx
  ) const {	
    return dtLinearAlgebra::length(
      _m1d.getPointPercent(xx[0]) - _m2d.getPointPercent(xx[1], xx[2])
    );

	}    
}
