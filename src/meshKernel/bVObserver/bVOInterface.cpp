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

  void bVOInterface::init( 
    ::QDomElement const & element,
		baseContainer const * const bC,
    cVPtrVec const * const cV,
    aFPtrVec const * const aF,
    aGPtrVec const * const aG,
    bVPtrVec const * const bV,
    boundedVolume * attachTo
  ) {
    optionHandling::init( element, bC, cV, aF, aG );
    _bV = attachTo;
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
}
