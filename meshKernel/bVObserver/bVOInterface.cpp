#include "bVOInterface.h"

#include <logMe/logMe.h>
#include <constValueHeaven/constValue.h>
#include <functionHeaven/analyticFunction.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <boundedVolume.h>

namespace dtOO {  
  bVOInterface::bVOInterface() {
    _bV = NULL;
  }

  bVOInterface::~bVOInterface() {
  
  }

  void bVOInterface::init( 
    QDomElement const & element,
		baseContainer const * const bC,
    vectorHandling< constValue * > const * const cV,
    vectorHandling< analyticFunction * > const * const aF,
    vectorHandling< analyticGeometry * > const * const aG,
    vectorHandling< boundedVolume * > const * const bV,
    boundedVolume * attachTo
  ) {
    _bV = attachTo;
  }  
  boundedVolume * bVOInterface::ptrBoundedVolume( void ) {
		return _bV;
	}
}
