#include "map1dTo3d.h"
#include <interfaceHeaven/staticPropertiesHandler.h>
#include <interfaceHeaven/calculationTypeHandling.h>
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

#include <analyticFunctionHeaven/scaLinearOneD.h>
#include "scaOneDInMap1dTo3d.h"

namespace dtOO { 
  float map1dTo3d::_deltaPer 
  = 
  staticPropertiesHandler::getInstance()->getOptionFloat("map1dTo3d_deltaPer");
  
  map1dTo3d::map1dTo3d() : analyticGeometry() {
  }

  map1dTo3d::map1dTo3d(const map1dTo3d& orig) : analyticGeometry(orig) {
  }

  map1dTo3d::~map1dTo3d() {
  }
  
  int map1dTo3d::dim( void ) const {
    return 1;
  }
  
  dtPoint3 map1dTo3d::getPoint( float const * const uvw ) const {
    return getPoint( *uvw );
  }
  
  dtPoint3 map1dTo3d::getPointPercent( float const & uu ) const {
    return getPoint( u_percent(uu) );
  }

  map1dTo3d * map1dTo3d::segmentPercent( 
    float const & u0, float const & u1 
  ) const {
    return segment( u_percent(u0), u_percent(u1) );
  }
  
  float map1dTo3d::u_percent(float const & uu) const {
    return floatHandling::boundToRange( 
      getUMin() +  (getUMax() - getUMin() ) * uu, getUMin(), getUMax()
    );    
  }  
  
  float map1dTo3d::percent_u(float const & uu) const {
    return floatHandling::boundToRange( 
      (uu - getUMin()) / (getUMax() - getUMin()), 0., 1.
    );    
  }
	
	float map1dTo3d::u_lPercent(float const & lP) const {
		return u_l( lP*length() );
	}
  
  float map1dTo3d::percent_l(float const & ll) const {
		return percent_u( u_l(ll) );
	}
	
  float map1dTo3d::lPercent_u(float const & uu) const {
		return percent_u( l_u(uu)/length() );
	}
  
  int map1dTo3d::getRenderResolutionU( void ) const { 
    return analyticGeometry::getRenderResolution(0);
  }  

  dtVector3 map1dTo3d::firstDerU( float const & uu) const {
    float uP = percent_u(uu);
    
    if (uP<0.01) {
      return (
        (getPointPercent(_deltaPer) - getPointPercent(0.))
        /
        (u_percent(_deltaPer) - u_percent(0.) )
      );      
    }
    else if ( (uP>=0.01) && (uP<=0.99) ) {
      return (
        ( getPointPercent(uP+_deltaPer) - getPointPercent(uP-_deltaPer) )
        /
        ( u_percent(uP+_deltaPer) - u_percent(uP-_deltaPer) )
      );
    }
    else if (uP>0.99) {
      return (
        (getPointPercent(1.) - getPointPercent(1.-_deltaPer))
        /
        (u_percent(1.) - u_percent(1.-_deltaPer) )
      );      
    }
  }

  dtVector3 map1dTo3d::firstDerUPercent( float const & uP) const {  
    return firstDerU( u_percent(uP) );
  }
  
  map1dTo3d * map1dTo3d::segment( float const & u0, float const & u1 ) const {
    scaLinearOneD fun(0, 1, u0, u1);
    return new scaOneDInMap1dTo3d(&fun, this);
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
    //
    // return if u = 0
    //
    if (uu == 0.) return 0.;
    
    //
    // determine length
    //
    std::vector< float > L(2, 0.0);
    dt__forFromToIndex(2, 4, nParts) {
      std::vector< float > curGrid = dtLinearAlgebra::unitGrid(nParts);
      dt__forFromToIndex(1, nParts, aPart) {
        std::vector< dtPoint2 > glp = dtLinearAlgebra::getGaussLegendre(nP);
        //float L = 0.0;
        float const u0 = curGrid[aPart-1] * uu;
        float const u1 = curGrid[aPart] * uu;
        const float rapJ = (u1 - u0) * .5;
        for (int i = 0; i < nP; i++){
          float const tt = glp[i].x();
          float const ww = glp[i].y();
          const float ui = u0 * 0.5 * (1. - tt) + u1 * 0.5 * (1. + tt);
          dtVector3 der = firstDerU(ui);
          const float d = sqrt(der.squared_length());
          L[1] += d * ww * rapJ;
        }
      }

      //
      // convergence check
      //
      if( fabs( (L[0] - L[1]) / L[1] ) < 1.e-3 ) return L[0];
      
      //
      // make new residual old
      //
      L[0] = L[1];
      L[1] = 0.;
    }
    
    return L[0];
	}

	float map1dTo3d::l_u( float const & uu ) const {
		return l_u(uu, 20);	
	}	

	double map1dTo3d::funValue(const double xx ) const {	
		float ll = l_u( static_cast<float>(xx) );
		return static_cast< double >( ll - _tmpL );
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
				dt__throw(u_l(), << "No iteration possible.");
			}
			theRoot = bisectF.Root();
		}

//		dt__info(
//			u_l(),
//			<< logMe::dtFormat(
//			     "mustIterate = %i, ll=%f, theRoot = %f, |ll-l_u(theRoot)| = %f"
//				 ) % mustIterate % ll % theRoot % fabs(ll-l_u(theRoot)) 
//		);		
	  return theRoot;
  }
	
  float map1dTo3d::operator%(const float &percent) const {
		return this->u_percent(percent);
	}
	
  float map1dTo3d::operator|(const float &length) const {
		return this->u_l(length);
	}
	
  float map1dTo3d::operator&(const float &lengthPercent) const {
		return this->u_lPercent(lengthPercent);
	}
	
	map1dTo3d * new_clone(map1dTo3d const & m1d) {
		return m1d.clone();
	}
}
