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

#include "x_vec3dClosestPointToPoint.h"

#include <interfaceHeaven/staticPropertiesHandler.h>
#include <analyticFunctionHeaven/vec3dFunction.h>
#include <dtAnalysis.h>
#include <Math/Functor.h>

namespace dtOO {
	x_vec3dClosestPointToPoint::x_vec3dClosestPointToPoint(
    vec3dFunction const * const v3d, aFY const & yy
	) : _v3d(v3d), _yy(yy) {

    // 
    // multidimensional minimization
    //
    dt__pH(dtMinimizer) min(
      dtAnalysis::createMinimizer(":Minuit2::kMigrad:")
    );
    ::ROOT::Math::Functor toMin;			
    if (v3d->xDim() == 1) {
      toMin = ::ROOT::Math::Functor(
        this, &x_vec3dClosestPointToPoint::FOne, 1
      );			
      min->SetFunction(toMin);          
      min->SetVariable( 0, "U", 0.5, 0.01 );
      min->SetVariableLimits(0, 0., 1.);
      
    }
    else if (v3d->xDim() == 2) {
      toMin = ::ROOT::Math::Functor(
        this, &x_vec3dClosestPointToPoint::FTwo, 2
      );			
      min->SetFunction(toMin);          
      //
      // set bounds
      //
      min->SetVariable( 0, "U", 0.5, 0.01 );
      min->SetVariableLimits(0, 0., 1.);
      min->SetVariable( 1, "V", 0.5, 0.01 );
      min->SetVariableLimits(1, 0., 1.);
    }
    else dt__throwUnexpected(x_vec3dClosestPointToPoint());    

    
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
    if (v3d->xDim() == 1) {
      _xx
      =
      _v3d->x_percent( analyticFunction::aFXOneD(theRoot[0]) );
    }
    else if (v3d->xDim() == 2) {
      _xx 
      = 
      _v3d->x_percent( analyticFunction::aFXTwoD(theRoot[0], theRoot[1]) );
    }
    else dt__throwUnexpected(x_vec3dClosestPointToPoint());
	}

	x_vec3dClosestPointToPoint::~x_vec3dClosestPointToPoint() {
	}
	
	aFX x_vec3dClosestPointToPoint::result( void ) {
		return _yy;
	}
  
  double x_vec3dClosestPointToPoint::FOne(
    double const * xx
  ) const {
    return
      dtLinearAlgebra::euclidianNorm( 
        dtLinearAlgebra::subtract(
          _v3d->Y( 
            _v3d->x_percent( analyticFunction::aFXOneD(xx[0]) ) 
          ), 
          _yy
        )
      );
	}
  
	double x_vec3dClosestPointToPoint::FTwo(
    double const * xx
  ) const {
    return
      dtLinearAlgebra::euclidianNorm( 
        dtLinearAlgebra::subtract(
          _v3d->Y( 
            _v3d->x_percent( analyticFunction::aFXTwoD(xx[0], xx[1]) ) 
          ), 
          _yy
        )
      );
	}      
}
