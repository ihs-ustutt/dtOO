#ifndef vec3dTwoDInMap3dTo3d_H
#define	vec3dTwoDInMap3dTo3d_H

#include "map2dTo3d.h"
#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/ptrHandling.h>

namespace dtOO {
  class vec3dTwoD;
  class vec3dSurfaceTwoD;
  class map3dTo3d;
  class renderInterface;
  
  class vec3dTwoDInMap3dTo3d : public map2dTo3d {
  public:
    dt__CLASSSTD(vec3dTwoDInMap3dTo3d, analyticGeometry);
    vec3dTwoDInMap3dTo3d();
    vec3dTwoDInMap3dTo3d(const vec3dTwoDInMap3dTo3d& orig);
    vec3dTwoDInMap3dTo3d(vec3dTwoD const * const v2d, map3dTo3d const * const m3d);
    virtual ~vec3dTwoDInMap3dTo3d();
    virtual map2dTo3d * clone( void ) const;
    virtual map2dTo3d * create( void ) const;
    virtual dtPoint3 getPoint( float const & uu, float const & vv ) const;
    virtual bool isClosed( int const & dir) const;
    virtual float getMin( int const & dir) const;
    virtual float getMax( int const & dir) const;
    vec3dTwoD const * refToVec3dTwoD( void ) const;
    map3dTo3d const * refToMap3dTo3d( void ) const;
    virtual map1dTo3d * pickConstUPercent(
      float const & uu, float const & vv0, float const & vv1
    ) const;
    virtual map1dTo3d * pickConstVPercent(
      float const & vv, float const & uu0, float const & uu1
    ) const;
    virtual map1dTo3d * pickLinearUV(
      float const & uu0, float const & vv0, 
      float const & uu1, float const & vv1
    ) const;
    virtual map2dTo3d * pickPercent(
      float const & uu0, float const & uu1, 
      float const & vv0, float const & vv1
    ) const;
    vectorHandling< renderInterface * > getExtRender( void ) const;
  private:
    ptrHandling< vec3dSurfaceTwoD > _v2d;
    ptrHandling< map3dTo3d > _m3d;
  };
}
#endif	/* vec3dTwoDInMap3dTo3d_H */

