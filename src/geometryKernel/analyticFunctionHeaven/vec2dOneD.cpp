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

#include "vec2dOneD.h"
#include "dtLinearAlgebra.h"
#include <interfaceHeaven/staticPropertiesHandler.h>

namespace dtOO {
	vec2dOneD::vec2dOneD() : vec2dFunction() {
	}

	vec2dOneD::vec2dOneD(const vec2dOneD& orig) : vec2dFunction(orig) {
		_min = orig._min;
		_max = orig._max;
	}

	vec2dOneD::~vec2dOneD() {
	}
	
	int vec2dOneD::xDim( void ) const {
		return 1;
	}

  aFY vec2dOneD::Y(aFX const & xx) const {	
		return Y(xx[0]);
	}

  dtPoint2 vec2dOneD::YdtPoint2(dtReal const & xx) const {
		return vec2dFunction::YdtPoint2( aFX(1, xx) );
	}
	
	dtPoint2 vec2dOneD::YdtPoint2Percent(dtReal const & xx) const {
		return YdtPoint2( x_percent(xx) );
	}
	
	dtReal vec2dOneD::x_percent(dtReal const & xx) const {
    return (xMin(0) +  (xMax(0) - xMin(0) ) * xx);
  }  
  
  dtReal vec2dOneD::percent_x(dtReal const & xx) const {
    return ( (xx - xMin(0)) / (xMax(0) - xMin(0)) );
  }
	
	dtVector2 vec2dOneD::DYdtVector2( dtReal const & xx ) const {
    dtReal xP = percent_x(xx);
    dtReal const deltaPer = 0.01;

		dtReal x0;
		dtReal x1;
    if (xP<0.01) {
			x0 = 0.;
      x1 = deltaPer;      
    }
    else if ( (xP>=deltaPer) && (xP<=(1.-deltaPer)) ) {
			x0 = xP-deltaPer;
			x1 = xP+deltaPer;
    }
    else if (xP>(1.-deltaPer)) {
			x0 = 1.-deltaPer;
			x1 = 1.;
    }		
		
		dtPoint2 y0 = YdtPoint2( x_percent(x0) );
		dtPoint2 y1 = YdtPoint2( x_percent(x1) );
		
		dtVector2 dxdy = (y1 - y0) / (x_percent(x1)-x_percent(x0) );
		
		return dxdy;
	}
	
	dtVector2 vec2dOneD::unitNdtVector2( dtReal const & xx ) const {
		return dtLinearAlgebra::unitNormal( DYdtVector2(xx) );
	}

	dtVector2 vec2dOneD::unitNdtVector2Percent( dtReal const & xx ) const {
		return dtLinearAlgebra::unitNormal( DYdtVector2( x_percent(xx) ) );
	}
	
  dtReal vec2dOneD::xMin( dtInt const & dir) const {
    switch (dir) {
      case 0:
        return _min;
        break;
      default:
        dt__throw(xMin(),
              << dt__eval(dir) << std::endl
              << "dir should be 0.");
    }   
	}
	
  dtReal vec2dOneD::xMax( dtInt const & dir) const {
    switch (dir) {
      case 0:
        return _max;
        break;
      default:
        dt__throw(xMax(),
              << dt__eval(dir) << std::endl
              << "dir should be 0.");
    }
	}	

  void vec2dOneD::setMin(dtReal const & min) {
    _min = min;
  }

  void vec2dOneD::setMax(dtReal const & max) {
    _max = max;
  }  	

  dtReal vec2dOneD::length( dtReal const & x1 ) const {	
	  dtInt glpOrder[16] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 20};
		dtReal l0 = length(1, x1);
		dtReal l1 = 0.;
		dtReal const geoRes 
		= 
		staticPropertiesHandler::getInstance()->getOptionFloat("xyz_resolution");
		for (int ii=0; ii<16; ii++) {
		  l1 = length(glpOrder[ii], x1);	
			dtReal eps = fabs(l1-l0)/l1;
			l0 = l1;
			if ( eps < geoRes ) {
			  break;
			}
		}		
		
		return l1;
	}
	
	dtReal vec2dOneD::length( void ) const {
		return length( xMax(0) );
	}
	
	dtReal vec2dOneD::length( dtInt const & nP, dtReal const & x1 ) const {
		std::vector< dtPoint2 > glp = dtLinearAlgebra::getGaussLegendre(nP);
		dtReal L = 0.0;
		dtReal const u0 = xMin(0);
		dtReal const u1 = x1;
		const dtReal rapJ = (u1 - u0) * .5;
		for (int i = 0; i < nP; i++){
			dtReal const tt = glp[i].x();
			dtReal const ww = glp[i].y();
			const dtReal ui = u0 * 0.5 * (1. - tt) + u1 * 0.5 * (1. + tt);
			dtVector2 der = DYdtVector2(ui);
			const dtReal d = sqrt(der.squared_length());
			L += d * ww * rapJ;
		}
		return L;
	}		
  
  dt__C_addCloneForpVH(vec2dOneD);          
}
