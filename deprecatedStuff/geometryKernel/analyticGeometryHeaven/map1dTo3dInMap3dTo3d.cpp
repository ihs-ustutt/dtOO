#include "map1dTo3dInMap3dTo3d.h"

#include <logMe/logMe.h>
#include "map3dTo3d.h"

namespace dtOO {
  map1dTo3dInMap3dTo3d::map1dTo3dInMap3dTo3d() : map1dTo3d() {
  }

  map1dTo3dInMap3dTo3d::map1dTo3dInMap3dTo3d(const map1dTo3dInMap3dTo3d& orig) : map1dTo3d(orig) {
    _m1d.reset( orig._m1d->clone() );
    _m3d.reset( orig._m3d->clone() );
  }

  map1dTo3dInMap3dTo3d::map1dTo3dInMap3dTo3d(
    map1dTo3d const * const m1d, 
    map3dTo3d const * const m3d
  ) : map1dTo3d() {
    _m1d.reset( m1d->clone() );
    _m3d.reset( m3d->clone() );
  }
  
  map1dTo3dInMap3dTo3d::~map1dTo3dInMap3dTo3d() {
  }

  map1dTo3dInMap3dTo3d * map1dTo3dInMap3dTo3d::clone( void ) const {
    return new map1dTo3dInMap3dTo3d(*this);
  }
  
  map1dTo3dInMap3dTo3d * map1dTo3dInMap3dTo3d::create( void ) const {
    return new map1dTo3dInMap3dTo3d();
  }
  
  dtPoint3 map1dTo3dInMap3dTo3d::getPoint( float const & uu ) const {
    dtPoint3 pUVW = _m1d->getPoint(uu);
    return _m3d->getPoint( pUVW.x(), pUVW.y(), pUVW.z() );    
  }
  
  map3dTo3d const * map1dTo3dInMap3dTo3d::getConstPtrToMap3dTo3d( void ) const {
    return _m3d.get();
  }

  map1dTo3d const * map1dTo3dInMap3dTo3d::getConstPtrToMap1dTo3d( void ) const {
    return _m1d.get();
  }  
  
  bool map1dTo3dInMap3dTo3d::isClosed( int const & dir) const {
    switch (dir) {
      case 0:
        return _m1d->isClosed(dir);
        break;
      default:
        dt__THROW(isClosed(),
              << DTLOGEVAL(dir) << LOGDEL
              << "dir should be 0.");
    }
  }
  
  float map1dTo3dInMap3dTo3d::getMin( int const & dir) const {
    switch (dir) {
      case 0:
        return _m1d->getMin(dir);
        break;
      default:
        dt__THROW(getMin(),
              << DTLOGEVAL(dir) << LOGDEL
              << "dir should be 0.");
    }    
  }

  float map1dTo3dInMap3dTo3d::getMax( int const & dir) const {
    switch (dir) {
      case 0:
        return _m1d->getMax(dir);
        break;
      default:
        dt__THROW(getMax(),
              << DTLOGEVAL(dir) << LOGDEL
              << "dir should be 0.");
    }    
  }    
}

