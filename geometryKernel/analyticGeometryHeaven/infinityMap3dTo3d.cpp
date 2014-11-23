#include "infinityMap3dTo3d.h"
#include <logMe/logMe.h>
#include "map2dTo3d.h"
#include "map1dTo3d.h"
#include "splineCurve3d.h"
#include "analyticSurface.h"
#include "rotatingSpline.h"
#include <geometryEngine/dtSurface.h>
#include <geometryEngine/geoBuilder/surfaceOfRevolution_curveRotateConstructOCC.h>
#include <geometryEngine/dtCurve.h>
#include <geometryEngine/geoBuilder/geomSurface_surfaceRotateConstructOCC.h>

namespace dtOO {
	infinityMap3dTo3d::infinityMap3dTo3d() : map3dTo3d() {
	}

	infinityMap3dTo3d::infinityMap3dTo3d(const infinityMap3dTo3d& orig) : map3dTo3d(orig) {
	}

	infinityMap3dTo3d::~infinityMap3dTo3d() {
	}
	
	infinityMap3dTo3d * infinityMap3dTo3d::clone( void ) const {
		return new infinityMap3dTo3d( *this );
	}
    
	infinityMap3dTo3d * infinityMap3dTo3d::create( void ) const {
		return new infinityMap3dTo3d();
	}
	
  bool infinityMap3dTo3d::isClosed( int const & dir) const {
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
        dt__THROW(isClosed(),
              << DTLOGEVAL(dir) << LOGDEL
              << "dir should be 0, 1 or 2.");
    }
	}
  
	float infinityMap3dTo3d::getMin( int const & dir) const {
    switch (dir) {
      case 0:
        return -1.;
        break;
      case 1:
        return -1.;
        break;
      case 2:
        return -1.;
        break;        
      default:
        dt__THROW(getMin(),
              << DTLOGEVAL(dir) << LOGDEL
              << "dir should be 0, 1 or 2.");
    }    		
	}
  
	float infinityMap3dTo3d::getMax( int const & dir) const {
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
        dt__THROW(getMax(),
              << DTLOGEVAL(dir) << LOGDEL
              << "dir should be 0, 1 or 2.");
    }    				
	}
    
	dtPoint3 infinityMap3dTo3d::getPoint( float const & uu, float const & vv, float const & ww ) const {	
    return dtPoint3(uu, vv, ww);
	}
}
