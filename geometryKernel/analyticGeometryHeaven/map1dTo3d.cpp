#include "map1dTo3d.h"
#include "interfaceHeaven/staticPropertiesHandler.h"
#include <logMe/logMe.h>
#include <interfaceHeaven/ptrHandling.h>
#include <solid3dLine.h>

namespace dtOO { 
  map1dTo3d::map1dTo3d() : analyticGeometry() {
  }

  map1dTo3d::map1dTo3d(const map1dTo3d& orig) : analyticGeometry(orig) {
  }

  map1dTo3d::~map1dTo3d() {
  }

  std::vector< dtPoint3 > map1dTo3d::getPointOrder( int const & nU ) const {
    std::vector< dtPoint3 > pp(nU);
    float intU = 1. / (static_cast<float>(nU)-1.);
    for (int jj=0; jj<nU; jj++) {
      float jjF = static_cast< float >(jj);
      pp[jj] = getPointPercent(jjF * intU);
    }
    
    return pp;
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
  
  std::vector< dtVector3 > map1dTo3d::firstDerUOrder( int const & nU ) const {
    std::vector< dtVector3 > pp(nU);
    float intU = 1. / (static_cast<float>(nU)-1.);
    for (int jj=0; jj<nU; jj++) {
      float jjF = static_cast< float >(jj);
      pp[jj] = firstDerU( u_percent(jjF * intU) );
    }
    
    return pp;
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
    std::vector<float> itVal;       
    std::vector< std::string > header;
    header.push_back("l0");
    header.push_back("l1");
    header.push_back("eps");		
		
	  int glpOrder[16] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 20};
		float l0 = l_u(uu, 1);
		float l1 = 0.;
		float const geoRes 
		= 
		staticPropertiesHandler::getInstance()->getOptionFloat("geometricResolution");
		for (int ii=0; ii<16; ii++) {
		  l1 = l_u(uu, glpOrder[ii]);	
			float eps = fabs(l1-l0)/l1;
			l0 = l1;
			itVal.push_back(l0);
			itVal.push_back(l1);
			itVal.push_back(eps);
			if ( eps < geoRes ) {
			  break;
			}
		}

    DTDEBUGWF( l_u(), << floatVecToTable(header, itVal) );		
		
		return l1;
	}	
}
