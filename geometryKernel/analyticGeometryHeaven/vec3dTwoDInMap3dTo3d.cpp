#include "vec3dTwoDInMap3dTo3d.h"

#include <logMe/logMe.h>
#include "map3dTo3d.h"
#include <interfaceHeaven/staticPropertiesHandler.h>
#include <functionHeaven/vec3dOneD.h>
#include <functionHeaven/vec3dCurveOneD.h>
#include <functionHeaven/vec3dCurve2dInSurfaceOneD.h>
#include <functionHeaven/vec3dTwoD.h>
#include <functionHeaven/vec3dSurfaceTwoD.h>
#include <geometryEngine/dtSurface.h>
#include <geometryEngine/dtCurve.h>
#include <geometryEngine/dtCurve2d.h>
#include <geometryEngine/geoBuilder/trimmedCurve2d_twoPointsConnectConstructOCC.h>
#include <geometryEngine/geoBuilder/rectangularTrimmedSurface_uvBounds.h>
#include "vec3dOneDInMap3dTo3d.h"

#include <progHelper.h>

namespace dtOO {    
  vec3dTwoDInMap3dTo3d::vec3dTwoDInMap3dTo3d() : map2dTo3d() {
  }

  vec3dTwoDInMap3dTo3d::vec3dTwoDInMap3dTo3d(const vec3dTwoDInMap3dTo3d& orig) : map2dTo3d(orig) { 
    _v2d.reset( orig._v2d->clone() );
    _m3d.reset( orig._m3d->clone() );
		
		labelHandling::setLabel( _v2d->getLabel()+"_in_"+_m3d->getLabel() );
  }

  vec3dTwoDInMap3dTo3d::vec3dTwoDInMap3dTo3d(vec3dTwoD const * const v2d, map3dTo3d const * const m3d) : map2dTo3d() {
		dt__PTRASS(
		  vec3dSurfaceTwoD const * v3d2d, 
			vec3dSurfaceTwoD::ConstDownCast(v2d)
		);
    _v2d.reset( v3d2d->clone() );
    _m3d.reset( m3d->clone() );
		
		labelHandling::setLabel( _v2d->getLabel()+"_in_"+_m3d->getLabel() );
  }

  vec3dTwoDInMap3dTo3d::~vec3dTwoDInMap3dTo3d() {
  }
  
  dtPoint3 vec3dTwoDInMap3dTo3d::getPoint( float const & uu, float const & vv ) const {
		aFX xx(2,0);
		xx[0] = uu; xx[1] = vv;
    dtPoint3 pUVW = _v2d->YdtPoint3(xx);
    return _m3d->getPoint( pUVW.x(), pUVW.y(), pUVW.z() );
  }
  
  bool vec3dTwoDInMap3dTo3d::isClosed( int const & dir) const {
		return _v2d->closed(dir);
  }
  
  float vec3dTwoDInMap3dTo3d::getMin( int const & dir) const {
    return _v2d->xMin(dir);    
  }

  float vec3dTwoDInMap3dTo3d::getMax( int const & dir) const {
    return _v2d->xMax(dir);    
  }

  map2dTo3d * vec3dTwoDInMap3dTo3d::create( void ) const {
    return new vec3dTwoDInMap3dTo3d();
  }
  
  map2dTo3d * vec3dTwoDInMap3dTo3d::clone( void ) const {
    return new vec3dTwoDInMap3dTo3d(*this);
  }
  
  map1dTo3d * vec3dTwoDInMap3dTo3d::pickConstUPercent(
    float const & uu, float const & vv0, float const & vv1
  ) const {
    ptrHandling< dtCurve > dtC(
		  _v2d->ptrDtSurface()->getCurveConstUPercent(uu, vv0, vv1)
		);
    ptrHandling< vec3dOneD > v1d( new vec3dCurveOneD(dtC.get()) );

    return new vec3dOneDInMap3dTo3d(v1d.get(), _m3d.get()); 
  }
  
  map1dTo3d * vec3dTwoDInMap3dTo3d::pickConstVPercent(
    float const & vv, float const & uu0, float const & uu1
  ) const {
    ptrHandling< dtCurve > dtC(
		  _v2d->ptrDtSurface()->getCurveConstVPercent(vv, uu0, uu1)
		);
    ptrHandling< vec3dOneD > v1d( new vec3dCurveOneD(dtC.get()) );

    return new vec3dOneDInMap3dTo3d(v1d.get(), _m3d.get());    
  }
  
  map1dTo3d * vec3dTwoDInMap3dTo3d::pickLinearUV(
    float const & uu0, float const & vv0, 
    float const & uu1, float const & vv1
  ) const {
		ptrHandling<dtCurve2d> dtC2d(
			trimmedCurve2d_twoPointsConnectConstructOCC(
				dtPoint2(uu0, vv0), dtPoint2(uu1, vv1)
			).result()
		);
		
		ptrHandling< vec3dOneD > v3d1d(
		  new vec3dCurve2dInSurfaceOneD( dtC2d.get(), _v2d->ptrDtSurface() ) 
		);
		
    return new vec3dOneDInMap3dTo3d(v3d1d.get(), _m3d.get());    
  }
  
  map2dTo3d * vec3dTwoDInMap3dTo3d::pickPercent(
    float const & uu0, float const & uu1, 
    float const & vv0, float const & vv1
  ) const {
    ptrHandling< dtSurface > dtS(
		  rectangularTrimmedSurface_uvBounds(
		    _v2d->ptrDtSurface(), 
				_v2d->ptrDtSurface()->uv_uvPercent(dtPoint2(uu0, vv0)), 
				_v2d->ptrDtSurface()->uv_uvPercent(dtPoint2(uu1, vv1))
		  ).result()
		);
    ptrHandling< vec3dTwoD > v2d( new vec3dSurfaceTwoD(dtS.get()) );

    return new vec3dTwoDInMap3dTo3d(v2d.get(), _m3d.get());   		
  }
  
  vec3dTwoD const * vec3dTwoDInMap3dTo3d::refToVec3dTwoD( void ) const {
    return _v2d.get();
  }
  
  map3dTo3d const * vec3dTwoDInMap3dTo3d::refToMap3dTo3d( void ) const {
    return _m3d.get();
  }  
}
