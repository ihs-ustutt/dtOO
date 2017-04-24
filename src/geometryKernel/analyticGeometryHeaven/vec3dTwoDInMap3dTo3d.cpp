#include "vec3dTwoDInMap3dTo3d.h"

#include <logMe/logMe.h>
#include "map3dTo3d.h"
#include <interfaceHeaven/staticPropertiesHandler.h>
#include <analyticFunctionHeaven/vec3dTwoD.h>
#include "map2dTo3dTransformed.h"
#include <discrete3dPoints.h>
#include <discrete3dVector.h>
#include <dtTransformerHeaven/dtTransformer.h>
#include <logMe/dtMacros.h>

namespace dtOO {    
  vec3dTwoDInMap3dTo3d::vec3dTwoDInMap3dTo3d() : map2dTo3d() {
  }

  vec3dTwoDInMap3dTo3d::vec3dTwoDInMap3dTo3d(
    const vec3dTwoDInMap3dTo3d& orig
  ) : map2dTo3d(orig) { 
    _v2d.reset( orig._v2d->clone() );
    _m3d.reset( orig._m3d->clone() );
		_percentF = orig._percentF;
  }
	
  vec3dTwoDInMap3dTo3d::vec3dTwoDInMap3dTo3d(    
	  vec3dTwoD const * const v2d, map3dTo3d const * const m3d, bool percentF
  ) {
    _v2d.reset( v2d->clone() );
    _m3d.reset( m3d->clone() );
		_percentF = percentF;
	}

  vec3dTwoDInMap3dTo3d::~vec3dTwoDInMap3dTo3d() {
  }
  
  dtPoint3 vec3dTwoDInMap3dTo3d::getPoint( 
    float const & uu, float const & vv 
  ) const {
		aFX xx(2, 0.);
		xx[0] = uu; xx[1] = vv;
    dtPoint3 pUVW = _v2d->YdtPoint3(xx);
		if (!_percentF) {
      return _m3d->getPoint( pUVW.x(), pUVW.y(), pUVW.z() );
		}
		else {
			return _m3d->getPointPercent( pUVW.x(), pUVW.y(), pUVW.z() );
		}		
  }
  
  bool vec3dTwoDInMap3dTo3d::isClosed( int const & dir) const {
		return _v2d->closed(dir);
  }
  
  float vec3dTwoDInMap3dTo3d::getMin( int const & dir) const {
    return _v2d->xMin(dir);    
  }

  float vec3dTwoDInMap3dTo3d::getMax( int const & dir) const {
    return _v2d->xMax(dir);    
  }

  vec3dTwoDInMap3dTo3d * vec3dTwoDInMap3dTo3d::clone( void ) const {
    return new vec3dTwoDInMap3dTo3d(*this);
  }
	
  vec3dTwoDInMap3dTo3d * vec3dTwoDInMap3dTo3d::create( void ) const {
    return new vec3dTwoDInMap3dTo3d();
  }
	
	vec3dTwoDInMap3dTo3d * vec3dTwoDInMap3dTo3d::cloneTransformed( 
    dtTransformer const * const dtT 
  ) const {
    return new map2dTo3dTransformed<vec3dTwoDInMap3dTo3d>(*this, dtT);
  }
  
  vec3dTwoD const * vec3dTwoDInMap3dTo3d::refToVec3dTwoD( void ) const {
    return _v2d.get();
  }
  
  map3dTo3d const * vec3dTwoDInMap3dTo3d::refToMap3dTo3d( void ) const {
    return _m3d.get();
  }  
  
	std::string vec3dTwoDInMap3dTo3d::dumpToString( void ) const {
		std::stringstream ss;
		
		ss
    << 
    dt__dumpToString( 
      << "_m3d = " << _m3d->virtualClassName() << std::endl
      << _m3d->dumpToString() << std::endl
      << "_v2d = " << _v2d->virtualClassName() << std::endl
      << _v2d->dumpToString() << std::endl
      << "_percentF = " << _percentF
    );    
    
		return ss.str();
	}
}
