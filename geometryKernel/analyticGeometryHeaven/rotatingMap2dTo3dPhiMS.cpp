#include "rotatingMap2dTo3dPhiMS.h"
#include <logMe/logMe.h>
#include "map2dTo3d.h"
#include "map1dTo3d.h"
#include "splineCurve3d.h"
#include <geometryEngine/dtCurve.h>

namespace dtOO {
	rotatingMap2dTo3dPhiMS::rotatingMap2dTo3dPhiMS() : rotatingMap2dTo3d() {
		_angle = 2. * M_PI;
	}

	rotatingMap2dTo3dPhiMS::rotatingMap2dTo3dPhiMS(const rotatingMap2dTo3dPhiMS& orig) : rotatingMap2dTo3d(orig) {
		_angle = orig._angle;
	}

	rotatingMap2dTo3dPhiMS::~rotatingMap2dTo3dPhiMS() {
	}
	
  rotatingMap2dTo3dPhiMS::rotatingMap2dTo3dPhiMS( dtVector3 const & vv, map2dTo3d const * const m2d ) : rotatingMap2dTo3d(vv, m2d) {
		_angle = 2. * M_PI;
	}
  
	float rotatingMap2dTo3dPhiMS::getMin( int const & dir) const {
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
        dt__THROW(getMin(),
              << DTLOGEVAL(dir) << LOGDEL
              << "dir should be 0, 1 or 2.");
    }    		
	}
  
	float rotatingMap2dTo3dPhiMS::getMax( int const & dir) const {
    switch (dir) {
      case 0:
        return _angle;
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
    
	dtPoint3 rotatingMap2dTo3dPhiMS::getPoint( float const & uu, float const & vv, float const & ww ) const {
		return rotatingMap2dTo3d::getPoint( u_phi(uu), v_m(vv), w_s(ww) );
	}	
	
  void rotatingMap2dTo3dPhiMS::setAngle( float const & angle ) {
		_angle = angle;
	}
}
