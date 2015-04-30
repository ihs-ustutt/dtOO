#include "dtPoint3_straightIntersectToMap2dTo3d.h"

#include <analyticGeometryHeaven/map2dTo3d.h>
#include <RConfigure.h>
#include <Rtypes.h>
#include <Math/GSLMinimizer.h>
#include <Math/GSLRootFinder.h>
#include <Math/RootFinderAlgorithms.h>
#include <Math/Functor.h>
#include <GSLError.h>

namespace dtOO {
	dtPoint3_straightIntersectToMap2dTo3d::dtPoint3_straightIntersectToMap2dTo3d(
    dtPoint3 const & p0, dtVector3 const & v0, map2dTo3d const * const m2d	
	) {
		_p0 = p0;
		_v0 = v0;
		_m2d = m2d;
		
		// 
		// multidimensional minimization
		//
		ROOT::Math::GSLMinimizer min( ROOT::Math::kVectorBFGS );
		ROOT::Math::Functor toMin(
			this, &dtPoint3_straightIntersectToMap2dTo3d::F, 2 
		);			
		min.SetFunction(toMin);

		//
		// set bounds
		//
		min.SetVariable( 0, "u", .5, .001 );			
		min.SetVariable( 1, "v", .5, .001 );			
		min.SetVariableLimits(0, 0., 1.);
		min.SetVariableLimits(1, 0., 1.);

		//
		// minimizer options
		//
		min.SetMaxFunctionCalls(1000000);
		min.SetMaxIterations(100000);
		min.SetTolerance(0.0000001);			
		min.SetPrintLevel(3);

		//
		// minimize
		//
		{
			dt__pipeCout(sp, cs)
			min.Minimize();
		}

		//
		// create points and bSpline
		//
		double const * const theRoot = min.X( );

		_pp = _m2d->getPointPercent(theRoot[0], theRoot[1]);		
	}

	dtPoint3_straightIntersectToMap2dTo3d::~dtPoint3_straightIntersectToMap2dTo3d(
	) {
		
	}
	
  dtPoint3 dtPoint3_straightIntersectToMap2dTo3d::result( void ) {
		return _pp;
	}

	double dtPoint3_straightIntersectToMap2dTo3d::F(
	  double const * xx 
	) const {	
		return 
		  1.
			-
			dtLinearAlgebra::dotProduct(
			  _v0, 
				dtLinearAlgebra::normalize(
				  _m2d->getPointPercent(dtPoint2(xx[0], xx[1])) - _p0
				)
			);
	}	
}
