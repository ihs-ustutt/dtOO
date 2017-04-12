#include "doNothing.h"
#include <logMe/logMe.h>

namespace dtOO {
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
	
  std::vector< dtPoint2 * > doNothing::apply( std::vector< dtPoint2 * > const * const pointVecP ) const {
  }

  std::vector< dtPoint3 * > doNothing::apply( std::vector< dtPoint3 * > const * const pointVecP ) const {

  }

  aGPtrVec doNothing::apply( aGPtrVec const * const aGeoVecP ) const {

  }

  aFPtrVec doNothing::apply( aFPtrVec const * const sFunVecP ) const {

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
