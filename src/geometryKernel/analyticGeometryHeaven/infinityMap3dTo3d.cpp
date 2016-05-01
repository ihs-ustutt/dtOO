#include "infinityMap3dTo3d.h"
#include <logMe/logMe.h>
#include "map3dTo3dTransformed.h"
#include <dtTransformerHeaven/dtTransformer.h>

namespace dtOO {
	infinityMap3dTo3d::infinityMap3dTo3d() : map3dTo3d() {
    
	}

	infinityMap3dTo3d::infinityMap3dTo3d(
    const infinityMap3dTo3d& orig
  ) : map3dTo3d(orig) {
    
	}

	infinityMap3dTo3d::~infinityMap3dTo3d() {
    
	}
	
	infinityMap3dTo3d * infinityMap3dTo3d::create( void ) const {
		return new infinityMap3dTo3d();
	}
  
	infinityMap3dTo3d * infinityMap3dTo3d::clone( void ) const {
		return new infinityMap3dTo3d( *this );
	}
    
  infinityMap3dTo3d * infinityMap3dTo3d::cloneTransformed( 
    dtTransformer const * const dtT  
  ) const {
    return new map3dTo3dTransformed< infinityMap3dTo3d >(*this, dtT);
  }  
	
  bool infinityMap3dTo3d::isClosed( int const & dir) const {
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
  
	float infinityMap3dTo3d::getMin( int const & dir) const {
    switch (dir) {
      case 0:
        return -1.;
      case 1:
        return -1.;
      case 2:
        return -1.;    
      default:
        dt__throw(getMin(),
              << dt__eval(dir) << std::endl
              << "dir should be 0, 1 or 2.");
    }    		
	}
  
	float infinityMap3dTo3d::getMax( int const & dir) const {
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
    
	dtPoint3 infinityMap3dTo3d::getPoint( 
    float const & uu, float const & vv, float const & ww 
  ) const {	
    return dtPoint3(uu, vv, ww);
	}
}
