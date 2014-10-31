#include "doNothing.h"
#include <logMe/logMe.h>

namespace dtOO {
  doNothing::doNothing() {
  }

  doNothing::~doNothing() {
  }

  std::vector< dtPoint2 * > doNothing::apply( std::vector< dtPoint2 * > const * const pointVecP ) const {
  }

  std::vector< dtPoint3 * > doNothing::apply( std::vector< dtPoint3 * > const * const pointVecP ) const {

  }

  vectorHandling< analyticGeometry * > doNothing::apply( vectorHandling< analyticGeometry * > const * const aGeoVecP ) const {

  }

  vectorHandling< analyticFunction * > doNothing::apply( vectorHandling< analyticFunction * > const * const sFunVecP ) const {

  }

  bool doNothing::isNecessary( void ) const {
    return false;
  }
}
