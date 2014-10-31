#include "vec3dOneDInMap3dTo3d.h"

#include <logMe/logMe.h>
#include "map1dTo3d.h"
#include "map3dTo3d.h"
#include <functionHeaven/vec3dOneD.h>
#include <interfaceHeaven/ptrVectorHandling.h>
#include <progHelper.h>

namespace dtOO {    
  vec3dOneDInMap3dTo3d::vec3dOneDInMap3dTo3d() : map1dTo3d() {
  }

  vec3dOneDInMap3dTo3d::vec3dOneDInMap3dTo3d(const vec3dOneDInMap3dTo3d& orig) : map1dTo3d(orig) { 
    _v1d.reset( orig._v1d->clone() );
    _m3d.reset( orig._m3d->clone() );
		
		labelHandling::setLabel( _v1d->getLabel()+"_in_"+_m3d->getLabel() );
  }

  vec3dOneDInMap3dTo3d::vec3dOneDInMap3dTo3d(vec3dOneD const * const v1d, map3dTo3d const * const m3d) : map1dTo3d() {
    _v1d.reset( v1d->clone() );
    _m3d.reset( m3d->clone() );
		
		labelHandling::setLabel( _v1d->getLabel()+"_in_"+_m3d->getLabel() );
  }

  vec3dOneDInMap3dTo3d::~vec3dOneDInMap3dTo3d() {
  }
  
  dtPoint3 vec3dOneDInMap3dTo3d::getPoint( float const & uu ) const {
    dtPoint3 pUVW = _v1d->YdtPoint3(uu);
    return _m3d->getPoint( pUVW.x(), pUVW.y(), pUVW.z() );
  }
  
  bool vec3dOneDInMap3dTo3d::isClosed( int const & dir) const {
    return _v1d->closed(dir);
  }
  
  float vec3dOneDInMap3dTo3d::getMin( int const & dir) const {
    return _v1d->xMin(dir);    
  }

  float vec3dOneDInMap3dTo3d::getMax( int const & dir) const {
    return _v1d->xMax(dir);    
  }
  
  map1dTo3d * vec3dOneDInMap3dTo3d::create( void ) const {
    return new vec3dOneDInMap3dTo3d();
  }
  
  map1dTo3d * vec3dOneDInMap3dTo3d::clone( void ) const {
    return new vec3dOneDInMap3dTo3d(*this);
  }
    
  vec3dOneD const * vec3dOneDInMap3dTo3d::refToVec3dOneD( void ) const {
    return _v1d.get();
  }
  
  map3dTo3d const * vec3dOneDInMap3dTo3d::refToMap3dTo3d( void ) const {
    return _m3d.get();
  }  
}
