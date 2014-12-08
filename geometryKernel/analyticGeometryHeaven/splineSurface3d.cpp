#include "splineSurface3d.h"

#include <logMe/logMe.h>
//#include "splineCurve3d.h"
#include <interfaceHeaven/ptrHandling.h>
#include <geometryEngine/dtCurve.h>
#include <geometryEngine/dtSurface.h>
#include <progHelper.h>
#include "map2dTo3dTransformed.h"
#include <dtTransformerHeaven/dtTransformer.h>
namespace dtOO {
  splineSurface3d::splineSurface3d()  : analyticSurface() {
  }

  splineSurface3d::splineSurface3d(dtSurface const * const surface) : analyticSurface(surface) {
  }

  splineSurface3d::splineSurface3d( splineSurface3d const & orig ) : analyticSurface(orig) {

  }
 
  splineSurface3d * splineSurface3d::clone( void ) const {
    return new splineSurface3d( *this );
  }

  splineSurface3d * splineSurface3d::create( void ) const {
    return new splineSurface3d();
  }
	
  splineSurface3d * splineSurface3d::cloneTransformed( dtTransformer const * const dtT ) const {
		return new map2dTo3dTransformed<splineSurface3d>(*this, dtT);
	}	
  
  splineSurface3d::~splineSurface3d() {
  }
}



