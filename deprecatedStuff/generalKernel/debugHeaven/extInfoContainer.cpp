#include "extInfoContainer.h"

#include <logMe/logMe.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <analyticFunctionHeaven/scaMuParserOneD.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <do/coDoSet.h>
#include <analyticFunctionHeaven/scaMuParserOneD.h>
#include <interfaceHeaven/stringPrimitive.h>
#include <progHelper.h>

namespace dtOO {
  extInfoContainer::extInfoContainer() {
  }

  extInfoContainer::extInfoContainer(const extInfoContainer& orig) {
  }

  extInfoContainer::~extInfoContainer() {
    _sFun.destroy();
    _aGeo.destroy();
  }
  
  void extInfoContainer::clear( void ) {
    _sFun.destroy();
    _aGeo.destroy();
  }
  
  void extInfoContainer::add( extInfoContainer const * const eIC ) {
    for (int ii=0;ii<eIC->_sFun.size();ii++) {
      _sFun.push_back( scaOneD::SecureCast(eIC->_sFun[ii]->clone())  );
    }
    for (int ii=0;ii<eIC->_aGeo.size();ii++) {
      _aGeo.push_back( eIC->_aGeo[ii]->clone()  );
    }    
  }
  
  void extInfoContainer::addNoClone( analyticFunction * sFunP ) {
    _sFun.push_back( sFunP );
  }  

  void extInfoContainer::addNoClone( analyticGeometry * aGeoP ) {
    _aGeo.push_back( aGeoP );
  }    

  void extInfoContainer::add( analyticGeometry const * const aGeoP ) {
    addNoClone( aGeoP->clone() );
  }  
  
  /**
   * @todo: Fix empty analyticFunction vector? Currenty one dummy function is added.
   *        Farmer solution!!
   */
  bool extInfoContainer::isEmpty( void ) const {
    if (_sFun.size() == 0) {
      return true;
    }
    return false;
  }

  coDoSetHandling< analyticFunction * > & extInfoContainer::getRefToScaFunction( void ) {
    return _sFun;
  }
  
  int extInfoContainer::getNumScaFunctions( void ) const {
    return _sFun.size();
  }
  
  coDoSetHandling< analyticGeometry * > & extInfoContainer::getRefToAnGeo( void ) {
    return _aGeo;
  }
    
  int extInfoContainer::getNumAnGeos( void ) const {
    return _aGeo.size();
  }
}
