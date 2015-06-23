#include "analyticGeometry.h"
#include <logMe/logMe.h>
#include <interfaceHeaven/staticPropertiesHandler.h>

namespace dtOO {
  analyticGeometry::analyticGeometry() : labelHandling() {
		_resU 
		=
		staticPropertiesHandler::getInstance()->getOptionInt(
      "geometry_render_resolution_u"
    );		
		_resV
		=
		staticPropertiesHandler::getInstance()->getOptionInt(
      "geometry_render_resolution_v"
    );		
		_resW
		=
		staticPropertiesHandler::getInstance()->getOptionInt(
      "geometry_render_resolution_w"
    );						
  }

  analyticGeometry::analyticGeometry( analyticGeometry const & orig ) : labelHandling(orig) {
    _resU = orig._resU;
    _resV = orig._resV;
		_resW = orig._resW;
  }

  analyticGeometry::~analyticGeometry() {
  }

	void analyticGeometry::setRenderResolution(int const & dir, int const & value) const {
    switch (dir) {
      case 0:
				_resU = value;
        return;
      case 1:
				_resV = value;
        return;
      case 2:
				_resW = value;
        return;
      default:
        dt__throw(setRenderResolution(),
              << dt__eval(dir) << std::endl
              << "dir should be 0, 1 or 2.");
    }		
	}
	
	
	int analyticGeometry::getRenderResolution(int const & dir) const {
    switch (dir) {
      case 0:
				return _resU;
      case 1:
				return _resV;
      case 2:
				return _resW;
      default:
        dt__throw(getRenderResolution(),
              << dt__eval(dir) << std::endl
              << "dir should be 0, 1 or 2.");
    }		
	}

  void analyticGeometry::dump( void ) const {
    dt__info(
			dump(), 
			<< dt__eval(getLabel()) << std::endl
		  << dt__eval(virtualClassName()) << std::endl
			<< "dumpToString()" << std::endl
			<< dumpToString()
		);
  }
	
	std::string analyticGeometry::dumpToString(void) const {
		return std::string("");
	}

  bool analyticGeometry::isTransformed( void ) const {
		return false;
	}
    
	bool analyticGeometry::isCompound( void ) const {
		return false;
	}

	vectorHandling< analyticGeometry const * > analyticGeometry::compoundInternal( void ) const {
		return vectorHandling< analyticGeometry const * >();
	}		
	
  bool analyticGeometry::inXYZTolerance(
    dtPoint3 const & p0, dtPoint3 const & p1, bool output
  ) const {
		float xyzResolution 
		= 
		staticPropertiesHandler::getInstance()->getOptionFloat(
      "xyz_resolution"
    );    		
    
		dtVector3 dist = p0 - p1;
		if (sqrt(dist.squared_length()) > xyzResolution) {		
      if (output) {
        dt__warning(
          inXYZTolerance(), 
          << dt__eval( getLabel() ) << std::endl
          << dt__point3d(p0) << std::endl
          << dt__point3d(p1) << std::endl
          << dt__eval( dtLinearAlgebra::length( dist) )
        );
      }
      return false;
    }

    return true;    
  }
  
  analyticGeometry * new_clone(analyticGeometry const & aG) {
    return aG.clone();
  }	
}