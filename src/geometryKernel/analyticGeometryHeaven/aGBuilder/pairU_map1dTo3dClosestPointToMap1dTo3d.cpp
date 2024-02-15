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

#include "pairU_map1dTo3dClosestPointToMap1dTo3d.h"

#include <interfaceHeaven/staticPropertiesHandler.h>
#include <analyticGeometryHeaven/map1dTo3d.h>
#include <dtAnalysis.h>
#include <Math/Functor.h>

namespace dtOO {
	pairU_map1dTo3dClosestPointToMap1dTo3d
    ::pairU_map1dTo3dClosestPointToMap1dTo3d(
      map1dTo3d const * const m1d0, map1dTo3d const * const m1d1
	) : _m1d0(*m1d0), _m1d1(*m1d1) {

    // 
    // multidimensional minimization
    //
    dt__pH(dtMinimizer) min(
      dtAnalysis::createMinimizer(":Minuit2::kMigrad:")
    );
    ::ROOT::Math::Functor toMin(
      this, &pairU_map1dTo3dClosestPointToMap1dTo3d::F, 2
    );			
    min->SetFunction(toMin);          
    
    //
    // set bounds
    //
    min->SetVariable( 0, "U_0", 0.5, 0.01 );
    min->SetVariableLimits(0, 0., 1.);
    min->SetVariable( 1, "U_1", 0.5, 0.01 );
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
    
    _closestU.first = ( _m1d0 % theRoot[0] );
    _closestU.second = ( _m1d1 % theRoot[1] );
	}

	pairU_map1dTo3dClosestPointToMap1dTo3d::~pairU_map1dTo3dClosestPointToMap1dTo3d() {
	}
	
	std::pair< dtReal, dtReal > pairU_map1dTo3dClosestPointToMap1dTo3d::result( void ) {
		return _closestU;
	}
  
	double pairU_map1dTo3dClosestPointToMap1dTo3d::F(
    double const * xx
  ) const {	
    return dtLinearAlgebra::length(
      _m1d0.getPointPercent(xx[0]) - _m1d1.getPointPercent(xx[1])
    );

	}    
}
