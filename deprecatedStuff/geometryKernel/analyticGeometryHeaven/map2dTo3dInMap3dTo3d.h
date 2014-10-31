#ifndef map2dTo3dInMap3dTo3d_H
#define	map2dTo3dInMap3dTo3d_H

#include "map2dTo3d.h"
#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/ptrHandling.h>

namespace dtOO {
  class map1dTo3d;
  class map3dTo3d;
  
  class map2dTo3dInMap3dTo3d : public map2dTo3d {
  public:
    dt__CLASSSTD(map2dTo3dInMap3dTo3d, analyticGeometry);
    map2dTo3dInMap3dTo3d();
    map2dTo3dInMap3dTo3d(const map2dTo3dInMap3dTo3d& orig);
    map2dTo3dInMap3dTo3d(map2dTo3d const * const m2d, map3dTo3d const * const m3d);
    virtual ~map2dTo3dInMap3dTo3d();
    virtual map2dTo3d * clone( void ) const;
    virtual map2dTo3d * create( void ) const;
    virtual dtPoint3 getPoint( float const & uu, float const & vv ) const;
    virtual bool isClosed( int const & dir) const;
    virtual float getMin( int const & dir) const;
    virtual float getMax( int const & dir) const;
//    virtual dtPoint2 reparamOnFace(dtPoint3 const & ppXYZ) const;
    map2dTo3d const * refToMap2dTo3d( void ) const;
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
  private:
    ptrHandling< map2dTo3d > _m2d;
    ptrHandling< map3dTo3d > _m3d;
  };
}
#endif	/* map2dTo3dInMap3dTo3d_H */

