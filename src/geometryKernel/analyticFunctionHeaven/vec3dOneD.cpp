#include "vec3dOneD.h"
#include "dtLinearAlgebra.h"
#include <interfaceHeaven/staticPropertiesHandler.h>
#include <solid3dLine.h>

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

  aFY vec3dOneD::Y(dtReal const & xx) const {	
		return Y(xx);
	}

  dtPoint3 vec3dOneD::YdtPoint3(dtReal const & xx) const {
		return vec3dFunction::YdtPoint3( aFX(1, xx) );
	}
	
	dtPoint3 vec3dOneD::YdtPoint3Percent(dtReal const & xx) const {
		return YdtPoint3( x_percent(xx) );
	}
	
	dtReal vec3dOneD::x_percent(dtReal const & xx) const {
    return (xMin(0) +  (xMax(0) - xMin(0) ) * xx);
  }  
  
  dtReal vec3dOneD::percent_x(dtReal const & xx) const {
    return ( (xx - xMin(0)) / (xMax(0) - xMin(0)) );
  }
	
	dtVector3 vec3dOneD::DYdtVector3( dtReal const & xx ) const {
    dtReal xP = percent_x(xx);
    dtReal const deltaPer = 0.01;

		dtReal x0;
		dtReal x1;
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
	
	dtVector3 vec3dOneD::DYdtVector3Percent(dtReal const & xx) const {
		return DYdtVector3( x_percent(xx) );
	}
	
  dtReal vec3dOneD::xMin( dtInt const & dir) const {
    switch (dir) {
      case 0:
        return _min;
        break;
      default:
        dt__throw(
          xMin(),
          << dt__eval(dir) << std::endl
          << "dir should be 0."
        );
    }   
	}
	
  dtReal vec3dOneD::xMax( dtInt const & dir) const {
    switch (dir) {
      case 0:
        return _max;
        break;
      default:
        dt__throw(
          xMax(),
          << dt__eval(dir) << std::endl
          << "dir should be 0."
        );
    }
	}	

  void vec3dOneD::setMin(dtReal const & min) {
    _min = min;
  }

  void vec3dOneD::setMax(dtReal const & max) {
    _max = max;
  }  	

  void vec3dOneD::setMin(int const & dir, dtReal const & min) {
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

  void vec3dOneD::setMax(int const & dir, dtReal const & max) {
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
  
  dtReal vec3dOneD::length( dtReal const & x1 ) const {
    std::vector<dtReal> itVal;       
    std::vector< std::string > header;
    header.push_back("l0");
    header.push_back("l1");
    header.push_back("eps");		
		
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
	
	dtReal vec3dOneD::length( void ) const {
		return length( xMax(0) );
	}
	
  dtReal vec3dOneD::operator%(const dtReal &percent) const {
		return x_percent(percent);
	}
	
  vectorHandling< renderInterface * > vec3dOneD::getRender( void ) const {
		int nU
		=
		staticPropertiesHandler::getInstance()->getOptionInt(
      "function_render_resolution_u"
    );		
		
		vectorHandling< dtPoint3 > p3(nU);
    dtReal interval = (xMax(0) - xMin(0)) / (nU-1);
    for (int ii=0;ii<nU;ii++) {
			dtReal iiF = static_cast<dtReal>(ii);
      dtReal xx = xMin(0) + iiF * interval;
      p3[ii] = YdtPoint3(xx);
    }
		
		vectorHandling< renderInterface * > rV(1);
		rV[0] = new solid3dLine(p3);
		
		return rV;
  }  

	dtReal vec3dOneD::length( dtInt const & nP, dtReal const & x1 ) const {
		std::vector< dtPoint2 > glp = dtLinearAlgebra::getGaussLegendre(nP);
		dtReal L = 0.0;
		dtReal const u0 = xMin(0);
		dtReal const u1 = x1;
		const dtReal rapJ = (u1 - u0) * .5;
		for (int i = 0; i < nP; i++){
			dtReal const tt = glp[i].x();
			dtReal const ww = glp[i].y();
			const dtReal ui = u0 * 0.5 * (1. - tt) + u1 * 0.5 * (1. + tt);
			dtVector3 der = DYdtVector3(ui);
			const dtReal d = sqrt(der.squared_length());
			L += d * ww * rapJ;
		}
		return L;
	}		
 
  dt__C_addCloneForpVH(vec3dOneD);   
}
