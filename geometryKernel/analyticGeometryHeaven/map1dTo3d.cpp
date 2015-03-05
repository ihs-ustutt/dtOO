#include "map1dTo3d.h"
#include "interfaceHeaven/staticPropertiesHandler.h"
#include "interfaceHeaven/floatHandling.h"
#include <logMe/logMe.h>
#include <interfaceHeaven/ptrHandling.h>
#include <solid3dLine.h>

#include <RConfigure.h>
#include <Rtypes.h>
#include <Math/GSLMinimizer.h>
#include <Math/GSLRootFinder.h>
#include <Math/RootFinderAlgorithms.h>
#include <Math/Functor.h>
#include <GSLError.h>

namespace dtOO { 
  map1dTo3d::map1dTo3d() : analyticGeometry() {
  }

  map1dTo3d::map1dTo3d(const map1dTo3d& orig) : analyticGeometry(orig) {
  }

  map1dTo3d::~map1dTo3d() {
  }
  
  dtPoint3 map1dTo3d::getPointPercent( float const & uu ) const {
    return getPoint( u_percent(uu) );
  }

  float map1dTo3d::u_percent(float const & uu) const {
    return (getUMin() +  (getUMax() - getUMin() ) * uu);
  }  
  
  float map1dTo3d::percent_u(float const & uu) const {
    return ( (uu - getUMin()) / (getUMax() - getUMin()) );
  }
	
	float map1dTo3d::u_lPercent(float const & lP) const {
		return u_l( lP*length() );
	}
  
  float map1dTo3d::percent_l(float const & ll) const {
		return percent_u( u_l(ll) );
	}
	
  int map1dTo3d::getRenderResolutionU( void ) const { 
    return analyticGeometry::getRenderResolution(0);
  }  

  dtVector3 map1dTo3d::firstDerU( float const & uu) const {
    float uP = percent_u(uu);
    float const deltaPer = 0.01;
    
    if (uP<0.01) {
      return (
        (getPointPercent(deltaPer) - getPointPercent(0.))
        /
        (u_percent(deltaPer) - u_percent(0.) )
      );      
    }
    else if ( (uP>=0.01) && (uP<=0.99) ) {
      return (
        ( getPointPercent(uP+deltaPer) - getPointPercent(uP-deltaPer) )
        /
        ( u_percent(uP+deltaPer) - u_percent(uP-deltaPer) )
      );
    }
    else if (uP>0.99) {
      return (
        (getPointPercent(1.) - getPointPercent(1.-deltaPer))
        /
        (u_percent(1.) - u_percent(1.-deltaPer) )
      );      
    }
  }

  vectorHandling< renderInterface * > map1dTo3d::getRender( void ) const {
		vectorHandling< dtPoint3 > pp(getRenderResolutionU());
    float interval = (getUMax() - getUMin()) / (getRenderResolutionU()-1);
    for(int ii=0; ii<getRenderResolutionU(); ii++){
      float paraValue = getUMin() + ii * interval;
      pp[ii] = getPoint(paraValue);
    }
		vectorHandling< renderInterface * > rV;
		rV.push_back( new solid3dLine(pp) );

    return rV;
  }
  
  bool map1dTo3d::isClosedU( void ) const {
    return isClosed(0);
  }
  
  float map1dTo3d::getUMin( void ) const {
    return getMin(0);
  }
  
  float map1dTo3d::getUMax( void ) const {
    return getMax(0);
  }
	
	float map1dTo3d::length( void ) const {
		return l_u(getUMax());
	}
	
	float map1dTo3d::l_u( float const & uu, int const & nP ) const {
		std::vector<dtPoint2> glp = dtLinearAlgebra::getGaussLegendre(nP);
		float L = 0.0;
		float const u0 = getUMin();
		float const u1 = uu;
		const float rapJ = (u1 - u0) * .5;
		for (int i = 0; i < nP; i++){
			float const tt = glp[i].x();
			float const ww = glp[i].y();
			const float ui = u0 * 0.5 * (1. - tt) + u1 * 0.5 * (1. + tt);
			dtVector3 der = firstDerU(ui);
			const float d = sqrt(der.squared_length());
			L += d * ww * rapJ;
		}
		return L;
	}

	float map1dTo3d::l_u( float const & uu ) const {
		return l_u(uu, 20);	
	}	

	double map1dTo3d::funValue(const double xx ) const {	
		float ll = l_u( static_cast<float>(xx) );
		return static_cast< double >( ll -_tmpL );
	}
	
  float map1dTo3d::u_l( float const & ll ) const {
		bool mustIterate = true;
		float lMax = length();
		float theRoot;

		if ( floatHandling::isSmall(ll) || (ll<0.) ) { 
			mustIterate = false;
			theRoot = getUMin();
		}
		else if ( floatHandling::isSmall(lMax-ll) || (ll>lMax) ) {
			mustIterate = false;
			theRoot = getUMax();
		}
		
		if (mustIterate) {
			_tmpL = ll;

			// Create the Integrator
			bool check = false;
			ROOT::Math::Roots::Bisection bisectF;
			ROOT::Math::Functor1D f0(this, &map1dTo3d::funValue );

			check 
			= 
			bisectF.SetFunction(
				f0, static_cast<double>(getUMin()), static_cast<double>(getUMax())
			); 

			if ( check ) {
				check = bisectF.Solve();
			}
			else {
				dt__THROW(u_l(), << "No iteration possible.");
			}
			theRoot = bisectF.Root();
		}

//		DTINFOWF(
//			u_l(),
//			<< logMe::dtFormat(
//			     "mustIterate = %i, ll=%f, theRoot = %f, |ll-l_u(theRoot)| = %f"
//				 ) % mustIterate % ll % theRoot % fabs(ll-l_u(theRoot)) 
//		);		
	  return theRoot;
  }
	
}
