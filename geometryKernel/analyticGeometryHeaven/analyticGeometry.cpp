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
        dt__THROW(setRenderResolution(),
              << DTLOGEVAL(dir) << LOGDEL
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
        dt__THROW(getRenderResolution(),
              << DTLOGEVAL(dir) << LOGDEL
              << "dir should be 0, 1 or 2.");
    }		
	}

  void analyticGeometry::dump( void ) const {
    DTLOGCHAP(dumping);
    DTINFOWF(dump(), << DTLOGEVAL( getLabel() ) );
  }
   
  void analyticGeometry::packToExtInfoContainer( extInfoContainer * const eIC ) const {
  }
	
	bool analyticGeometry::isCompound( void ) const {
		return false;
	}

	vectorHandling< analyticGeometry const * > analyticGeometry::compoundInternal( void ) const {
		return vectorHandling< analyticGeometry const * >();
	}		
}