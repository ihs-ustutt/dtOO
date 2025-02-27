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

#include "map1dTo3d.h"
#include <interfaceHeaven/staticPropertiesHandler.h>
#include <interfaceHeaven/calculationTypeHandling.h>
#include <logMe/logMe.h>
#include <interfaceHeaven/ptrHandling.h>
#include <solid3dLine.h>

#include <attributionHeaven/map1dTo3dLength.h>
#include <gslMinFloatAttr.h>

#include <analyticFunctionHeaven/scaLinearOneD.h>
#include "scaOneDInMap1dTo3d.h"
#include "aGBuilder/dtPoint3_map1dTo3dPoint.h"

namespace dtOO { 
  dtReal map1dTo3d::_deltaPer 
  = 
  staticPropertiesHandler::getInstance()->getOptionFloat("map1dTo3d_deltaPer");
  
  map1dTo3d::map1dTo3d() : analyticGeometry() {
  }

  map1dTo3d::map1dTo3d(const map1dTo3d& orig) : analyticGeometry(orig) {
  }

  map1dTo3d::~map1dTo3d() {
  }
  
  dtInt map1dTo3d::dim( void ) const {
    return 1;
  }
  
  dtPoint3 map1dTo3d::getPoint( dtReal const * const uvw ) const {
    return getPoint( *uvw );
  }
  
  dtPoint3 map1dTo3d::getPointPercent( dtReal const & uu ) const {
    return getPoint( u_percent(uu) );
  }

  map1dTo3d * map1dTo3d::segmentPercent( 
    dtReal const & u0, dtReal const & u1 
  ) const {
    return segment( u_percent(u0), u_percent(u1) );
  }
  
  dtReal map1dTo3d::u_percent(dtReal const & uu) const {
    return floatHandling::boundToRange( 
      getUMin() +  (getUMax() - getUMin() ) * uu, getUMin(), getUMax()
    );    
  }  
  
  dtReal map1dTo3d::percent_u(dtReal const & uu) const {
    return floatHandling::boundToRange( 
      (uu - getUMin()) / (getUMax() - getUMin()), 0., 1.
    );    
  }
	
	dtReal map1dTo3d::u_lPercent(dtReal const & lP) const {
		return u_l( lP*length() );
	}
  
  dtReal map1dTo3d::percent_l(dtReal const & ll) const {
		return percent_u( u_l(ll) );
	}
	
  dtReal map1dTo3d::lPercent_u(dtReal const & uu) const {
		return percent_u( l_u(uu)/length() );
	}
  
  dtInt map1dTo3d::getRenderResolutionU( void ) const { 
    return analyticGeometry::getRenderResolution(0);
  }  

  dtVector3 map1dTo3d::firstDerU( dtReal const & uu) const {
    dtReal uP = percent_u(uu);
    
    if (uP<0.01) {
      return (
        (getPointPercent(_deltaPer) - getPointPercent(0.))
        /
        (u_percent(_deltaPer) - u_percent(0.) )
      );      
    }
    if (uP>0.99) {
      return (
        (getPointPercent(1.) - getPointPercent(1.-_deltaPer))
        /
        (u_percent(1.) - u_percent(1.-_deltaPer) )
      );      
    }
    return (
      ( getPointPercent(uP+_deltaPer) - getPointPercent(uP-_deltaPer) )
      /
      ( u_percent(uP+_deltaPer) - u_percent(uP-_deltaPer) )
    );
  }

  dtVector3 map1dTo3d::firstDerUPercent( dtReal const & uP) const {  
    return firstDerU( u_percent(uP) );
  }
  
  map1dTo3d * map1dTo3d::segment( dtReal const & u0, dtReal const & u1 ) const {
    scaLinearOneD fun(0, 1, u0, u1);
    return new scaOneDInMap1dTo3d(&fun, this);
  }
  
  vectorHandling< renderInterface * > map1dTo3d::getRender( void ) const {
		vectorHandling< renderInterface * > rV;
		rV.push_back( 
      new solid3dLine(
        dtPoint3_map1dTo3dPoint(this, getRenderResolutionU()).result()
      ) 
    );

    return rV;
  }
  
  bool map1dTo3d::isClosedU( void ) const {
    return isClosed(0);
  }
  
  dtReal map1dTo3d::getUMin( void ) const {
    return getMin(0);
  }
  
  dtReal map1dTo3d::getUMax( void ) const {
    return getMax(0);
  }
	
	dtReal map1dTo3d::length( void ) const {
		return l_u(getUMax());
	}
	
	dtReal map1dTo3d::l_u( dtReal const & uu, dtInt const & nP ) const {
    //
    // return if u = 0
    //
    if (uu == 0.) return 0.;
    
    //
    // determine length
    //
    std::vector< dtReal > L(2, 0.0);
    dt__forFromToIndex(2, 4, nParts) {
      std::vector< dtReal > curGrid = dtLinearAlgebra::unitGrid(nParts);
      dt__forFromToIndex(1, nParts, aPart) {
        std::vector< dtPoint2 > glp = dtLinearAlgebra::getGaussLegendre(nP);
        //dtReal L = 0.0;
        dtReal const u0 = curGrid[aPart-1] * uu;
        dtReal const u1 = curGrid[aPart] * uu;
        const dtReal rapJ = (u1 - u0) * .5;
        for (int i = 0; i < nP; i++){
          dtReal const tt = glp[i].x();
          dtReal const ww = glp[i].y();
          const dtReal ui = u0 * 0.5 * (1. - tt) + u1 * 0.5 * (1. + tt);
          dtVector3 der = firstDerU(ui);
          const dtReal d = sqrt(der.squared_length());
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

	dtReal map1dTo3d::l_u( dtReal const & uu ) const {
		return l_u(uu, 20);	
	}	

	double map1dTo3d::funValue(const double xx ) const {	
		dtReal ll = l_u( static_cast<dtReal>(xx) );
		return static_cast< double >( ll - _tmpL );
	}
	
  dtReal map1dTo3d::u_l( dtReal const & ll ) const {
		dtReal lMax = length();
		dtReal theRoot;

		if ( floatHandling::isSmall(ll) || (ll<0.) ) { 
			theRoot = getUMin();
		}
		else if ( floatHandling::isSmall(lMax-ll) || (ll>lMax) ) {
			theRoot = getUMax();
    }
    else {
      gslMinFloatAttr md(
        new map1dTo3dLength(this, ll),
        0.5,
        0.001,
        staticPropertiesHandler::getInstance()->getOptionFloat(
          "xyz_resolution"
        )
      );
      md.perform();
      dt__throwIf(!md.converged(), u_l());
      theRoot = md.result()[0];
		}
	  return u_percent( theRoot );
  }
	
  dtReal map1dTo3d::operator%(const dtReal &percent) const {
		return this->u_percent(percent);
	}
	
  dtReal map1dTo3d::operator|(const dtReal &length) const {
		return this->u_l(length);
	}
	
  dtReal map1dTo3d::operator&(const dtReal &lengthPercent) const {
		return this->u_lPercent(lengthPercent);
	}
	
	map1dTo3d * new_clone(map1dTo3d const & m1d) {
		return m1d.clone();
	}
}
