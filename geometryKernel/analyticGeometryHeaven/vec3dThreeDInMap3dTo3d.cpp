#include "vec3dThreeDInMap3dTo3d.h"

#include <logMe/logMe.h>
#include "map3dTo3d.h"
#include <interfaceHeaven/staticPropertiesHandler.h>
#include <analyticFunctionHeaven/vec3dThreeD.h>
#include <progHelper.h>

namespace dtOO {    
  vec3dThreeDInMap3dTo3d::vec3dThreeDInMap3dTo3d() : map3dTo3d() {
  }

  vec3dThreeDInMap3dTo3d::vec3dThreeDInMap3dTo3d(
    const vec3dThreeDInMap3dTo3d& orig
  ) : map3dTo3d(orig) { 
    _v3d.reset( orig._v3d->clone() );
    _m3d.reset( orig._m3d->clone() );
		_percentF = orig._percentF;
		labelHandling::setLabel( _v3d->getLabel()+"_in_"+_m3d->getLabel() );
  }

  vec3dThreeDInMap3dTo3d::vec3dThreeDInMap3dTo3d(
	  vec3dThreeD const * const v3d, 
		map3dTo3d const * const m3d, 
		bool percentF
	) : map3dTo3d() {
    _v3d.reset( v3d->clone() );
    _m3d.reset( m3d->clone() );
		_percentF = percentF;
		
		labelHandling::setLabel( _v3d->getLabel()+"_in_"+_m3d->getLabel() );
  }	

  vec3dThreeDInMap3dTo3d::~vec3dThreeDInMap3dTo3d() {
  }
  
  dtPoint3 vec3dThreeDInMap3dTo3d::getPoint( 
    float const & uu, float const & vv, float const & ww 
  ) const {
		aFX xx(3,0);
		xx[0] = uu; xx[1] = vv; xx[2] = ww;
    dtPoint3 pUVW = _v3d->YdtPoint3(xx);
		if (!_percentF) {
      return _m3d->getPoint( pUVW.x(), pUVW.y(), pUVW.z() );
		}
		else {
			return _m3d->getPointPercent( pUVW.x(), pUVW.y(), pUVW.z() );
		}
  }
  
  bool vec3dThreeDInMap3dTo3d::isClosed( int const & dir) const {
		return false;
  }
  
  float vec3dThreeDInMap3dTo3d::getMin( int const & dir) const {
    return _v3d->xMin(dir);    
  }

  float vec3dThreeDInMap3dTo3d::getMax( int const & dir) const {
    return _v3d->xMax(dir);    
  }

  map3dTo3d * vec3dThreeDInMap3dTo3d::create( void ) const {
    return new vec3dThreeDInMap3dTo3d();
  }
  
  map3dTo3d * vec3dThreeDInMap3dTo3d::clone( void ) const {
    return new vec3dThreeDInMap3dTo3d(*this);
  }
  
  dtPoint3 vec3dThreeDInMap3dTo3d::reparamInVolume(
    dtPoint3 const & ppXYZ
  ) const {
    dtPoint3 ppUVW = _m3d->reparamInVolume(ppXYZ);
    aFX xx 
    = 
    _v3d->invY( analyticFunction::aFYThreeD(ppUVW) );
    
    dtPoint3 ppXYZReparam;
    if (!_percentF) ppXYZReparam = getPoint(xx[0], xx[1], xx[2]);
    else ppXYZReparam = getPointPercent(xx[0], xx[1], xx[2]);
    
    float dist = dtLinearAlgebra::length(ppXYZ - ppXYZReparam);
    dt__warnIfWithMessage(
      dist
      >
      staticPropertiesHandler::getInstance()->getOptionFloat("xyz_resolution"),
      reparamInVolume(), 
      << dt__eval(dist)
    );  
    return dtPoint3(xx[0], xx[1], xx[2]);
  }
}
