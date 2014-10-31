#include "map2dTo3dInMap3dTo3d.h"

#include <logMe/logMe.h>
#include "map3dTo3d.h"
#include "map1dTo3dInMap3dTo3d.h"
#include <interfaceHeaven/ptrVectorHandling.h>
#include <interfaceHeaven/staticPropertiesHandler.h>
#include <do/coDoSet.h>
#include <progHelper.h>

namespace dtOO {    
  map2dTo3dInMap3dTo3d::map2dTo3dInMap3dTo3d() : map2dTo3d() {
  }

  map2dTo3dInMap3dTo3d::map2dTo3dInMap3dTo3d(const map2dTo3dInMap3dTo3d& orig) : map2dTo3d(orig) { 
    _m2d.reset( orig._m2d->clone() );
    _m3d.reset( orig._m3d->clone() );
		
		labelHandling::setLabel( _m2d->getLabel()+"_in_"+_m3d->getLabel() );
  }

  map2dTo3dInMap3dTo3d::map2dTo3dInMap3dTo3d(map2dTo3d const * const m2d, map3dTo3d const * const m3d) : map2dTo3d() {
    _m2d.reset( m2d->clone() );
    _m3d.reset( m3d->clone() );
		
		labelHandling::setLabel( _m2d->getLabel()+"_in_"+_m3d->getLabel() );
  }

  map2dTo3dInMap3dTo3d::~map2dTo3dInMap3dTo3d() {
  }
  
  dtPoint3 map2dTo3dInMap3dTo3d::getPoint( float const & uu, float const & vv ) const {
    dtPoint3 pUVW = _m2d->getPoint(uu, vv);
    return _m3d->getPoint( pUVW.x(), pUVW.y(), pUVW.z() );
  }
  
  bool map2dTo3dInMap3dTo3d::isClosed( int const & dir) const {
    switch (dir) {
      case 0:
        return _m2d->isClosedU();
        break;
      case 1:
        return _m2d->isClosedV();
        break;    
      default:
        dt__THROW(isClosed(),
              << DTLOGEVAL(dir) << LOGDEL
              << "dir should be 0 or 1.");
    }
  }
  
  float map2dTo3dInMap3dTo3d::getMin( int const & dir) const {
    switch (dir) {
      case 0:
        return _m2d->getUMin();
        break;
      case 1:
        return _m2d->getVMin();
        break;
      default:
        dt__THROW(getMin(),
              << DTLOGEVAL(dir) << LOGDEL
              << "dir should be 0 or 1.");
    }    
  }

  float map2dTo3dInMap3dTo3d::getMax( int const & dir) const {
    switch (dir) {
      case 0:
        return _m2d->getUMax();
        break;
      case 1:
        return _m2d->getVMax();
        break;
      default:
        dt__THROW(getMax(),
              << DTLOGEVAL(dir) << LOGDEL
              << "dir should be 0 or 1.");
    }    
  }
  
//  dtPoint2 map2dTo3dInMap3dTo3d::reparamOnFace(dtPoint3 const & ppXYZ) const {
//    dtPoint3 ppUVW = _m3d->reparamInVolume(ppXYZ);
//    dtPoint2 ppUV = _m2d->reparamOnFace(ppUVW);
//    dtPoint3 ppXYZRep = map2dTo3d::getPoint(ppUV);
//    dtVector3 vvXYZ = ppXYZ - ppXYZRep;
//		
//		float minPDist 
//		= 
//		staticPropertiesHandler::getInstance()->getOptionFloat(
//      "minPointDistance"
//    );    		
//		if ( sqrt(vvXYZ.squared_length()) > minPDist) {
//			DTINFOWF(reparamOnFace(),
//        << DTLOGEVAL(getLabel()) << LOGDEL							
//				<< "Reparameterization of " << DTLOGPOI3D(ppXYZ) << ":" << LOGDEL
//				<< DTLOGPOI3D( ppXYZRep ) << LOGDEL
//				<< "distance = " << sqrt(vvXYZ.squared_length())
//			);
//		}
//    return ppUV;
//  }

  map2dTo3d * map2dTo3dInMap3dTo3d::create( void ) const {
    return new map2dTo3dInMap3dTo3d();
  }
  
  map2dTo3d * map2dTo3dInMap3dTo3d::clone( void ) const {
    return new map2dTo3dInMap3dTo3d(*this);
  }
  
  map1dTo3d * map2dTo3dInMap3dTo3d::pickConstUPercent(
    float const & uu, float const & vv0, float const & vv1
  ) const {
    ptrHandling< map1dTo3d > m1d( _m2d->pickConstUPercent(uu, vv0, vv1) );
    
    return new map1dTo3dInMap3dTo3d(m1d.get(), _m3d.get());
  }
  
  map1dTo3d * map2dTo3dInMap3dTo3d::pickConstVPercent(
    float const & vv, float const & uu0, float const & uu1
  ) const {
    ptrHandling< map1dTo3d > m1d( _m2d->pickConstVPercent(vv, uu0, uu1) );
    
    return new map1dTo3dInMap3dTo3d(m1d.get(), _m3d.get());    
  }
  
  map1dTo3d * map2dTo3dInMap3dTo3d::pickLinearUV(
    float const & uu0, float const & vv0, 
    float const & uu1, float const & vv1
  ) const {
    ptrHandling< map1dTo3d > m1d( _m2d->pickLinearUV(uu0, vv0, uu1, vv1) );
    
    return new map1dTo3dInMap3dTo3d(m1d.get(), _m3d.get());    
  }
  
  map2dTo3d * map2dTo3dInMap3dTo3d::pickPercent(
    float const & uu0, float const & uu1, 
    float const & vv0, float const & vv1
  ) const {
    ptrHandling< map2dTo3d > m2d( _m2d->pickPercent(uu0, uu1, vv0, vv1) );
    
    return new map2dTo3dInMap3dTo3d(m2d.get(), _m3d.get());    
  }
  
  map2dTo3d const * map2dTo3dInMap3dTo3d::refToMap2dTo3d( void ) const {
    return _m2d.get();
  }
  
  map3dTo3d const * map2dTo3dInMap3dTo3d::refToMap3dTo3d( void ) const {
    return _m3d.get();
  }  
}
