#include "partRotatingMap2dTo3d.h"
#include "map3dTo3dTransformed.h"

namespace dtOO {
	partRotatingMap2dTo3d::partRotatingMap2dTo3d() : rotatingMap2dTo3d() {
	}

	partRotatingMap2dTo3d::partRotatingMap2dTo3d(
    const partRotatingMap2dTo3d& orig
  ) : rotatingMap2dTo3d(orig) {
    _minB = orig._minB;
    _maxB = orig._maxB;
	}

	partRotatingMap2dTo3d::~partRotatingMap2dTo3d() {
	}
	
  partRotatingMap2dTo3d::partRotatingMap2dTo3d( 
    dtVector3 const & vv, map2dTo3d const * const m2d, 
    dtReal const & minB, dtReal const & maxB
  ) : rotatingMap2dTo3d(vv, m2d) {
    _minB = minB;
    _maxB = maxB;
	}
  
	partRotatingMap2dTo3d * partRotatingMap2dTo3d::create( void ) const {
		return new partRotatingMap2dTo3d();
	}
  
	partRotatingMap2dTo3d * partRotatingMap2dTo3d::clone( void ) const {
		return new partRotatingMap2dTo3d( *this );
	}
  
  partRotatingMap2dTo3d * partRotatingMap2dTo3d::cloneTransformed( 
    dtTransformer const * const dtT  
  ) const {
    return new map3dTo3dTransformed< partRotatingMap2dTo3d >(*this, dtT);
  }  
	
  bool partRotatingMap2dTo3d::isClosed( dtInt const & dir) const {
    switch (dir) {
      case 0:
        return false;
      default:
        return rotatingMap2dTo3d::isClosed( dir );
    }
	}
  
	dtReal partRotatingMap2dTo3d::getMin( dtInt const & dir) const {
    switch (dir) {
      case 0:
        return _minB;
      default:
        return rotatingMap2dTo3d::getMin( dir );
    }    		
	}
  
	dtReal partRotatingMap2dTo3d::getMax( dtInt const & dir) const {
    switch (dir) {
      case 0:
        return _maxB;
      default:
        return rotatingMap2dTo3d::getMax( dir );
    }    		
	}
    
	dtPoint3 partRotatingMap2dTo3d::getPoint( 
    dtReal const & uu, dtReal const & vv, dtReal const & ww 
  ) const {
	  return rotatingMap2dTo3d::getPoint( uu, vv, ww );	
	}	
}
