#include "trans6SidedCube.h"

#include <logMe/logMe.h>
#include <analyticFunctionHeaven/transIntCube.h>
#include "map2dTo3d.h"
#include <interfaceHeaven/stringPrimitive.h>
#include <interfaceHeaven/ptrVectorHandling.h>
#include "map3dTo3dTransformed.h"
#include <dtTransformerHeaven/dtTransformer.h>

namespace dtOO { 
  trans6SidedCube::trans6SidedCube() : map3dTo3d() {
    
  }

  trans6SidedCube::trans6SidedCube(const trans6SidedCube& orig) : map3dTo3d(orig) {
    _tI.reset( new transIntCube( *(orig._tI.get()) ) );
  }
  
  trans6SidedCube::trans6SidedCube(
    map2dTo3d const * const aS0, 
    map2dTo3d const * const aS1,
    map2dTo3d const * const aS2, 
    map2dTo3d const * const aS3, 
    map2dTo3d const * const aS4,
    map2dTo3d const * const aS5
  ) : map3dTo3d() {
    _tI.reset( new transIntCube(aS0, aS1, aS2, aS3, aS4, aS5) );
  }

  trans6SidedCube * trans6SidedCube::create( void ) const {
    return new trans6SidedCube();
  }
  
  trans6SidedCube * trans6SidedCube::clone( void ) const {
    return new trans6SidedCube(*this);
  }
  
  trans6SidedCube * trans6SidedCube::cloneTransformed( 
    dtTransformer const * const dtT  
  ) const {
    return new map3dTo3dTransformed< trans6SidedCube >(*this, dtT);
  }
    
  trans6SidedCube::~trans6SidedCube() {
    
  }
  
  dtPoint3 trans6SidedCube::getPoint( 
    float const & uu, float const & vv, float const & ww 
  ) const {
    return _tI->getValue(uu, vv, ww);
  }
  
  bool trans6SidedCube::isClosed( int const & dir) const {
    switch (dir) {
      case 0:
        return false;
      case 1:
        return false;
      case 2:
        return false;      
      default:
        dt__throw(isClosed(),
              << dt__eval(dir) << std::endl
              << "dir should be 0, 1 or 2.");
    }
  }
  
  float trans6SidedCube::getMin( int const & dir) const {
    switch (dir) {
      case 0:
        return 0.;
      case 1:
        return 0.;
      case 2:
        return 0.;    
      default:
        dt__throw(getMin(),
              << dt__eval(dir) << std::endl
              << "dir should be 0, 1 or 2.");
    }    
  }
  
  float trans6SidedCube::getMax( int const & dir) const {
    switch (dir) {
      case 0:
        return 1.;
      case 1:
        return 1.;
      case 2:
        return 1.;    
      default:
        dt__throw(getMax(),
              << dt__eval(dir) << std::endl
              << "dir should be 0, 1 or 2.");
    }    
  }

  map2dTo3d * trans6SidedCube::segmentConstU( float const & uu ) const {
    if ( analyticGeometry::inUVWTolerance(uu, getUMin()) ) {
      return _tI->getConstRefToMap2dTo3d()[1]->clone();
    }
    else if ( analyticGeometry::inUVWTolerance(uu, getUMax()) ) {
      return _tI->getConstRefToMap2dTo3d()[3]->clone();
    }
    else {
      return map3dTo3d::segmentConstU(uu);
    }
  }
    
  map2dTo3d * trans6SidedCube::segmentConstV( float const & vv ) const {
    if ( analyticGeometry::inUVWTolerance(vv, getVMin()) ) {
      return _tI->getConstRefToMap2dTo3d()[4]->clone();      
    }
    else if ( analyticGeometry::inUVWTolerance(vv, getVMax()) ) {
      return _tI->getConstRefToMap2dTo3d()[5]->clone();      
    }
    else {
      return map3dTo3d::segmentConstV(vv);
    }
  }
    
  map2dTo3d * trans6SidedCube::segmentConstW( float const & ww ) const {
    if ( analyticGeometry::inUVWTolerance(ww, getWMin()) ) {
      return _tI->getConstRefToMap2dTo3d()[2]->clone();      
    }
    else if ( analyticGeometry::inUVWTolerance(ww, getWMax()) ) {
      return _tI->getConstRefToMap2dTo3d()[0]->clone();      
    }
    else {
      return map3dTo3d::segmentConstW(ww);
    }
  }
}
