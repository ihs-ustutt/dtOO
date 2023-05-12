#include "bVOInterface.h"

#include <logMe/logMe.h>
#include <constValueHeaven/constValue.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <boundedVolume.h>

namespace dtOO {  
  bVOInterface::bVOInterface() {
    _bV = NULL;
  }

  bVOInterface::~bVOInterface() {
  
  }

  void bVOInterface::jInit( 
    jsonPrimitive const & jE,
		baseContainer const * const bC,
    lvH_constValue const * const cV,
    lvH_analyticFunction const * const aF,
    lvH_analyticGeometry const * const aG,
    lvH_boundedVolume const * const bV,
    boundedVolume * attachTo
  ) {
    _config = jE;
    dt__debug( jInit(), << _config.toStdString() );
    optionHandling::jInit(_config);
    _bV = attachTo;
  }  

  void bVOInterface::init( 
    ::QDomElement const & element,
		baseContainer const * const bC,
    lvH_constValue const * const cV,
    lvH_analyticFunction const * const aF,
    lvH_analyticGeometry const * const aG,
    lvH_boundedVolume const * const bV,
    boundedVolume * attachTo
  ) {
    optionHandling::init( element, bC, cV, aF, aG );
    bVOInterface::jInit(jsonPrimitive(), bC, cV, aF, aG, bV, attachTo);
  }  

	
  std::vector< std::string > bVOInterface::factoryAlias( void ) const {
    return std::vector< std::string>(0);
  }  

  boundedVolume const & bVOInterface::constRefBoundedVolume( void ) {
		return *_bV;
	}
  
  boundedVolume * bVOInterface::ptrBoundedVolume( void ) {
		return _bV;
	}

  void bVOInterface::preUpdate( void ) {
		
	}
	
  void bVOInterface::postUpdate( void ) {
		
	}

  jsonPrimitive & bVOInterface::config( void ) {
    return _config;
  }
  
  jsonPrimitive const & bVOInterface::config( void ) const {
    return _config;
  }
}
