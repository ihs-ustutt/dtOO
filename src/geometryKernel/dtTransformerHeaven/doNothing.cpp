#include "doNothing.h"
#include <logMe/logMe.h>

#include "dtTransformerFactory.h"
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <analyticFunctionHeaven/analyticFunction.h>

namespace dtOO {
  bool doNothing::_registrated 
  =
  dtTransformerFactory::registrate(
    dt__tmpPtr(doNothing, new doNothing())
  );
  
  doNothing::doNothing() : dtTransformer() {
  }

  doNothing::~doNothing() {
  }

  dtTransformer * doNothing::clone( void ) const {
	  dt__throw(clone(), << "Not yet implemented.");
	}
	
  dtTransformer * doNothing::create( void ) const {
		return new doNothing();
	}
	
  std::vector< dtPoint2 * > doNothing::apply( 
    std::vector< dtPoint2 * > const * const pointVecP 
  ) const {
    std::vector< dtPoint2 * > ret;
    dt__forAllRefAuto( *pointVecP, aPoint) ret.push_back( aPoint );
    return ret;
  }

  std::vector< dtPoint3 * > doNothing::apply( 
    std::vector< dtPoint3 * > const * const pointVecP 
  ) const {
    std::vector< dtPoint3 * > ret;
    dt__forAllRefAuto( *pointVecP, aPoint) ret.push_back( aPoint );
    return ret;
  }

  aGPtrVec doNothing::apply( aGPtrVec const * const aGeoVecP ) const {
    aGPtrVec ret;
    dt__forAllRefAuto( *aGeoVecP, aGeo) ret.push_back(aGeo->clone());
    return ret;
  }

  aFPtrVec doNothing::apply( aFPtrVec const * const sFunVecP ) const {
    aFPtrVec ret;
    dt__forAllRefAuto( *sFunVecP, aFun) ret.push_back(aFun->clone());
    return ret;
  }

	void doNothing::init( 
		::QDomElement const * tE, 
		baseContainer * const bC,
		cVPtrVec const * const cValP,
		aFPtrVec const * const sFunP,
		aGPtrVec const * const depAGeoP 
	) {
		
	}
		
  bool doNothing::isNecessary( void ) const {
    return false;
  }
}
