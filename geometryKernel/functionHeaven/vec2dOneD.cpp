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

  dtPoint2 vec2dOneD::YdtPoint2(float const & xx) const {
		return vec2dFunction::YdtPoint2( aFX(1, xx) );
	}
	
	dtPoint2 vec2dOneD::YdtPoint2Percent(float const & xx) const {
		return YdtPoint2( x_percent(xx) );
	}
	
	float vec2dOneD::x_percent(float const & xx) const {
    return (xMin(0) +  (xMax(0) - xMin(0) ) * xx);
  }  
  
  float vec2dOneD::percent_x(float const & xx) const {
    return ( (xx - xMin(0)) / (xMax(0) - xMin(0)) );
  }
	
	dtVector2 vec2dOneD::DYdtVector2( float const & xx ) const {
    float xP = percent_x(xx);
    float const deltaPer = 0.01;

		float x0;
		float x1;
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
	
	dtVector2 vec2dOneD::unitNdtVector2( float const & xx ) const {
		return dtLinearAlgebra::unitNormal( DYdtVector2(xx) );
	}

	dtVector2 vec2dOneD::unitNdtVector2Percent( float const & xx ) const {
		return dtLinearAlgebra::unitNormal( DYdtVector2( x_percent(xx) ) );
	}
	
  float vec2dOneD::xMin( int const & dir) const {
    switch (dir) {
      case 0:
        return _min;
        break;
      default:
        dt__THROW(xMin(),
              << DTLOGEVAL(dir) << LOGDEL
              << "dir should be 0.");
    }   
	}
	
  float vec2dOneD::xMax( int const & dir) const {
    switch (dir) {
      case 0:
        return _max;
        break;
      default:
        dt__THROW(xMax(),
              << DTLOGEVAL(dir) << LOGDEL
              << "dir should be 0.");
    }
	}	

  void vec2dOneD::setMin(float const & min) {
    _min = min;
  }

  void vec2dOneD::setMax(float const & max) {
    _max = max;
  }  	

  float vec2dOneD::length( float const & x1 ) const {	
	  int glpOrder[16] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 20};
		float l0 = length(1, x1);
		float l1 = 0.;
		float const geoRes 
		= 
		staticPropertiesHandler::getInstance()->getOptionFloat("xyz_resolution");
		for (int ii=0; ii<16; ii++) {
		  l1 = length(glpOrder[ii], x1);	
			float eps = fabs(l1-l0)/l1;
			l0 = l1;
			if ( eps < geoRes ) {
			  break;
			}
		}		
		
		return l1;
	}
	
	float vec2dOneD::length( void ) const {
		return length( xMax(0) );
	}
	
	float vec2dOneD::length( int const & nP, float const & x1 ) const {
		std::vector< dtPoint2 > glp = dtLinearAlgebra::getGaussLegendre(nP);
		float L = 0.0;
		float const u0 = xMin(0);
		float const u1 = x1;
		const float rapJ = (u1 - u0) * .5;
		for (int i = 0; i < nP; i++){
			float const tt = glp[i].x();
			float const ww = glp[i].y();
			const float ui = u0 * 0.5 * (1. - tt) + u1 * 0.5 * (1. + tt);
			dtVector2 der = DYdtVector2(ui);
			const float d = sqrt(der.squared_length());
			L += d * ww * rapJ;
		}
		return L;
	}		
}
