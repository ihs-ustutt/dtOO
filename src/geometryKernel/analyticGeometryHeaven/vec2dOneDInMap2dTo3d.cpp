#include "vec2dOneDInMap2dTo3d.h"
#include <logMe/logMe.h>
#include "map2dTo3d.h"
#include <analyticFunctionHeaven/vec2dOneD.h>
#include "map1dTo3dTransformed.h"
#include <dtTransformerHeaven/dtTransformer.h>

namespace dtOO { 
	vec2dOneDInMap2dTo3d::vec2dOneDInMap2dTo3d() : map1dTo3d() {

	}

	vec2dOneDInMap2dTo3d::vec2dOneDInMap2dTo3d(
    vec2dOneD const * const v2d1d, map2dTo3d const * const m2d
  ) : map1dTo3d() {
		_v2d.reset( v2d1d->clone() );
		_m2d.reset( m2d->clone() );
		_percentF = false;
	}
	
	vec2dOneDInMap2dTo3d::vec2dOneDInMap2dTo3d(
    vec2dOneD const * const v2d1d, 
    map2dTo3d const * const m2d, 
    bool const & percentF
  ) : map1dTo3d() {
		_v2d.reset( v2d1d->clone() );
		_m2d.reset( m2d->clone() );
		_percentF = percentF;
	}

	vec2dOneDInMap2dTo3d::vec2dOneDInMap2dTo3d(
    const vec2dOneDInMap2dTo3d& orig
  ) : map1dTo3d(orig) {
		_v2d.reset( orig._v2d->clone() );
		_m2d.reset( orig._m2d->clone() );	
		_percentF = orig._percentF;
	}

	vec2dOneDInMap2dTo3d::~vec2dOneDInMap2dTo3d() {

	}

	vec2dOneDInMap2dTo3d * vec2dOneDInMap2dTo3d::clone( void ) const {
    return new vec2dOneDInMap2dTo3d(*this);
	}

  vec2dOneDInMap2dTo3d * vec2dOneDInMap2dTo3d::cloneTransformed( 
    dtTransformer const * const dtT 
  ) const {
		return new map1dTo3dTransformed<vec2dOneDInMap2dTo3d>(*this, dtT);
	}    

	vec2dOneDInMap2dTo3d * vec2dOneDInMap2dTo3d::create( void ) const {
		return new vec2dOneDInMap2dTo3d();
	}

	dtPoint3 vec2dOneDInMap2dTo3d::getPoint( dtReal const & uu ) const {
		if (_percentF) {
			return _m2d->getPoint( _m2d->uv_percent(_v2d->YdtPoint2(uu)) );
		}
		else {
      return _m2d->getPoint( _v2d->YdtPoint2(uu) );
		}
  }

	bool vec2dOneDInMap2dTo3d::isClosed( dtInt const & dir) const {
    return _v2d->closed(dir);
	}

	dtReal vec2dOneDInMap2dTo3d::getMin( dtInt const & dir) const {
		return _v2d->xMin(dir);     
	}

	dtReal vec2dOneDInMap2dTo3d::getMax( dtInt const & dir) const {
		return _v2d->xMax(dir);
	}

  vec2dOneD const * vec2dOneDInMap2dTo3d::ptrToVec2dOneD( void ) const {
	  return _v2d.get();	
	}
	
  map2dTo3d const * vec2dOneDInMap2dTo3d::ptrToMap2dTo3d( void ) const {
		return _m2d.get();
	}
	
	bool vec2dOneDInMap2dTo3d::isPercent( void ) const {
		return _percentF;
	}
}