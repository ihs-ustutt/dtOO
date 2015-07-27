#include "vec2dTwoDInMap2dTo3d.h"
#include <logMe/logMe.h>
#include "map2dTo3d.h"
#include <analyticFunctionHeaven/vec2dTwoD.h>
#include "map2dTo3dTransformed.h"
#include <dtTransformerHeaven/dtTransformer.h>
#include <interfaceHeaven/staticPropertiesHandler.h>

namespace dtOO { 
	vec2dTwoDInMap2dTo3d::vec2dTwoDInMap2dTo3d() : map2dTo3d() {
	}

	vec2dTwoDInMap2dTo3d::vec2dTwoDInMap2dTo3d(
	  vec2dTwoD const * const v2d2d, map2dTo3d const * const m2d
	) : map2dTo3d() {
		_v2d.reset( v2d2d->clone() );
		_m2d.reset( m2d->clone() );
		_percentF = false;
	}
	
	vec2dTwoDInMap2dTo3d::vec2dTwoDInMap2dTo3d(
	  vec2dTwoD const * const v2d2d, map2dTo3d const * const m2d, 
		bool const & percentF
	) : map2dTo3d() {
		_v2d.reset( v2d2d->clone() );
		_m2d.reset( m2d->clone() );
		_percentF = percentF;
	}

	vec2dTwoDInMap2dTo3d::vec2dTwoDInMap2dTo3d(
	  const vec2dTwoDInMap2dTo3d& orig
	) : map2dTo3d(orig) {
		_v2d.reset( orig._v2d->clone() );
		_m2d.reset( orig._m2d->clone() );	
		_percentF = orig._percentF;
	}

	vec2dTwoDInMap2dTo3d::~vec2dTwoDInMap2dTo3d() {

	}

	vec2dTwoDInMap2dTo3d * vec2dTwoDInMap2dTo3d::clone( void ) const {
    return new vec2dTwoDInMap2dTo3d(*this);
	}

	vec2dTwoDInMap2dTo3d * vec2dTwoDInMap2dTo3d::create( void ) const {
		return new vec2dTwoDInMap2dTo3d();
	}

  vec2dTwoDInMap2dTo3d * vec2dTwoDInMap2dTo3d::cloneTransformed( 
	dtTransformer const * const dtT 
	) const {
		return new map2dTo3dTransformed<vec2dTwoDInMap2dTo3d>(*this, dtT);		
	}
			
	dtPoint3 vec2dTwoDInMap2dTo3d::getPoint( 
	  float const & uu, float const & vv 
	) const {
		if (_percentF) {
			return _m2d->getPoint( _m2d->uv_percent(_v2d->YdtPoint2(uu, vv)) );
		}
		else {
      return _m2d->getPoint( _v2d->YdtPoint2(uu, vv) );
		}
  }

	bool vec2dTwoDInMap2dTo3d::isClosed( int const & dir) const {
    return _v2d->closed(dir);
	}

	float vec2dTwoDInMap2dTo3d::getMin( int const & dir) const {
		return _v2d->xMin(dir);     
	}

	float vec2dTwoDInMap2dTo3d::getMax( int const & dir) const {
		return _v2d->xMax(dir);
	}
      
  vec2dTwoD const * vec2dTwoDInMap2dTo3d::ptrToVec2dTwoD( void ) const {
	  return _v2d.get();	
	}
	
  map2dTo3d const * vec2dTwoDInMap2dTo3d::ptrToMap2dTo3d( void ) const {
		return _m2d.get();
	}
}