#include "vec3dOneD.h"
#include "dtLinearAlgebra.h"
#include <interfaceHeaven/staticPropertiesHandler.h>

namespace dtOO {
	vec3dOneD::vec3dOneD() : vec3dFunction() {
	}

	vec3dOneD::vec3dOneD(const vec3dOneD& orig) : vec3dFunction(orig) {
		_min = orig._min;
		_max = orig._max;
	}

	vec3dOneD::~vec3dOneD() {
	}
	
	int vec3dOneD::xDim( void ) const {
		return 1;
	}

  aFY vec3dOneD::Y(float const & xx) const {	
		return Y(xx);
	}

  dtPoint3 vec3dOneD::YdtPoint3(float const & xx) const {
		return vec3dFunction::YdtPoint3( aFX(1, xx) );
	}
	
	dtPoint3 vec3dOneD::YdtPoint3Percent(float const & xx) const {
		return YdtPoint3( x_percent(xx) );
	}
	
	float vec3dOneD::x_percent(float const & xx) const {
    return (xMin(0) +  (xMax(0) - xMin(0) ) * xx);
  }  
  
  float vec3dOneD::percent_x(float const & xx) const {
    return ( (xx - xMin(0)) / (xMax(0) - xMin(0)) );
  }
	
	dtVector3 vec3dOneD::DYdtVector3( float const & xx ) const {
    float xP = percent_x(xx);
    float const deltaPer = 0.01;

		float x0;
		float x1;
    if (xP<0.01) {
			x0 = 0.;
      x1 = deltaPer;      
    }
    else if ( (xP>=0.01) && (xP<=0.99) ) {
			x0 = xP-deltaPer;
			x1 = xP+deltaPer;
    }
    else if (xP>0.99) {
			x0 = 1.-deltaPer;
			x1 = 1.;
    }		
		
		dtPoint3 y0 = YdtPoint3( x_percent(x0) );
		dtPoint3 y1 = YdtPoint3( x_percent(x1) );
		
		dtVector3 dxdy = (y1 - y0) / (x_percent(x1)-x_percent(x0) );
		
		return dxdy;
	}
	
	dtVector3 vec3dOneD::DYdtVector3Percent(float const & xx) const {
		return DYdtVector3( x_percent(xx) );
	}
	
  float vec3dOneD::xMin( int const & dir) const {
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
	
  float vec3dOneD::xMax( int const & dir) const {
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

  void vec3dOneD::setMin(float const & min) {
    _min = min;
  }

  void vec3dOneD::setMax(float const & max) {
    _max = max;
  }  	

  void vec3dOneD::setMin(int const & dir, float const & min) {
    switch (dir) {
      case 0:
        setMin(min);
        break;
      default:
        dt__throw(
          setMin(),
          << dt__eval(dir) << std::endl
          << "dir should be 0."
        );
    }
  }

  void vec3dOneD::setMax(int const & dir, float const & max) {
    switch (dir) {
      case 0:
        setMax(max);
        break;
      default:
        dt__throw(
          setMax(),
          << dt__eval(dir) << std::endl
          << "dir should be 0."
        );
    }
  }
  
  float vec3dOneD::length( float const & x1 ) const {
    std::vector<float> itVal;       
    std::vector< std::string > header;
    header.push_back("l0");
    header.push_back("l1");
    header.push_back("eps");		
		
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
			itVal.push_back(l0);
			itVal.push_back(l1);
			itVal.push_back(eps);
			if ( eps < geoRes ) {
			  break;
			}
		}

    dt__debug( length(), << logMe::vecToTable(header, itVal) );		
		
		return l1;
	}
	
	float vec3dOneD::length( void ) const {
		return length( xMax(0) );
	}
	
  float vec3dOneD::operator%(const float &percent) const {
		return x_percent(percent);
	}
	
	float vec3dOneD::length( int const & nP, float const & x1 ) const {
		std::vector< dtPoint2 > glp = dtLinearAlgebra::getGaussLegendre(nP);
		float L = 0.0;
		float const u0 = xMin(0);
		float const u1 = x1;
		const float rapJ = (u1 - u0) * .5;
		for (int i = 0; i < nP; i++){
			float const tt = glp[i].x();
			float const ww = glp[i].y();
			const float ui = u0 * 0.5 * (1. - tt) + u1 * 0.5 * (1. + tt);
			dtVector3 der = DYdtVector3(ui);
			const float d = sqrt(der.squared_length());
			L += d * ww * rapJ;
		}
		return L;
	}		
  
  dt__C_addCloneForpVH(vec3dOneD);   
}
