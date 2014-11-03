#include "discrete3dVector.h"

#include <logMe/logMe.h>
#include <interfaceHeaven/ptrHandling.h>
#include <interfaceHeaven/staticPropertiesHandler.h>

namespace dtOO {
	discrete3dVector::discrete3dVector() : renderInterface() {
	}

	discrete3dVector::~discrete3dVector() {
	}

	/**
	 * 
   * @todo Min/Max finding? Necessary?
   */
  discrete3dVector::discrete3dVector(vectorHandling< dtVector3 > const & vv, vectorHandling< dtPoint3 > const & pp) : renderInterface() {
		_v3 = vv;
		_p3 = pp;
	}
	
	vectorHandling< dtVector3 > const & discrete3dVector::refV3( void ) const {
		return _v3;
	}	
	
	vectorHandling< dtPoint3 > const & discrete3dVector::refP3( void ) const {
		return _p3;
	}		
}
