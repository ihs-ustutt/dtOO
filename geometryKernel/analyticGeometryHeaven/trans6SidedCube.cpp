#include "trans6SidedCube.h"

#include <logMe/logMe.h>
#include <analyticFunctionHeaven/transIntCube.h>
#include "map2dTo3d.h"
#include <interfaceHeaven/stringPrimitive.h>
#include <interfaceHeaven/ptrVectorHandling.h>

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

  trans6SidedCube * trans6SidedCube::clone( void ) const {
    return new trans6SidedCube(*this);
  }
  
  trans6SidedCube * trans6SidedCube::create( void ) const {
    return new trans6SidedCube();
  }
    
  trans6SidedCube::~trans6SidedCube() {
    
  }
  
  dtPoint3 trans6SidedCube::getPoint( float const & uu, float const & vv, float const & ww ) const {
    return _tI->getValue(uu, vv, ww);
  }
  
  bool trans6SidedCube::isClosed( int const & dir) const {
    switch (dir) {
      case 0:
        return false;
        break;
      case 1:
        return false;
        break;
      case 2:
        return false;
        break;        
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
        break;
      case 1:
        return 0.;
        break;
      case 2:
        return 0.;
        break;        
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
        break;
      case 1:
        return 1.;
        break;
      case 2:
        return 1.;
        break;        
      default:
        dt__throw(getMax(),
              << dt__eval(dir) << std::endl
              << "dir should be 0, 1 or 2.");
    }    
  }
}
