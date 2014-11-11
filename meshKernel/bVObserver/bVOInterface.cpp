#include "bVOInterface.h"

#include <logMe/logMe.h>

namespace dtOO {  
  bVOInterface::bVOInterface() {
    _bV = NULL;
  }

  bVOInterface::~bVOInterface() {
  
  }

  void bVOInterface::init( 
    QDomElement const & element,
    vectorHandling< constValue * > const * const cValP,
    vectorHandling< analyticFunction * > const * const sFunP,
    vectorHandling< analyticGeometry * > const * const depAGeoP,
    vectorHandling< boundedVolume * > const * const depBVolP,
    boundedVolume * bV
  ) {
    _bV = bV;
  }  
  boundedVolume * bVOInterface::ptrBoundedVolume( void ) {
		return _bV;
	}
}
