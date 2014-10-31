#ifndef vec3dOneDInMap3dTo3d_H
#define	vec3dOneDInMap3dTo3d_H

#include "map1dTo3d.h"
#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/ptrHandling.h>

namespace dtOO {
  class vec3dOneD;
  class map3dTo3d;
  
  class vec3dOneDInMap3dTo3d : public map1dTo3d {
  public:
    dt__CLASSSTD(vec3dOneDInMap3dTo3d, analyticGeometry);
    vec3dOneDInMap3dTo3d();
    vec3dOneDInMap3dTo3d(const vec3dOneDInMap3dTo3d& orig);
    vec3dOneDInMap3dTo3d(vec3dOneD const * const v1d, map3dTo3d const * const m3d);
    virtual ~vec3dOneDInMap3dTo3d();
    virtual map1dTo3d * clone( void ) const;
    virtual map1dTo3d * create( void ) const;
    virtual dtPoint3 getPoint( float const & uu ) const;
    virtual bool isClosed( int const & dir) const;
    virtual float getMin( int const & dir) const;
    virtual float getMax( int const & dir) const;
    vec3dOneD const * refToVec3dOneD( void ) const;
    map3dTo3d const * refToMap3dTo3d( void ) const;
  private:
    ptrHandling< vec3dOneD > _v1d;
    ptrHandling< map3dTo3d > _m3d;
  };
}
#endif	/* vec3dOneDInMap3dTo3d_H */

