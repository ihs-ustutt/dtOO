#ifndef vec3dThreeDInMap3dTo3d_H
#define	vec3dThreeDInMap3dTo3d_H

#include "map3dTo3d.h"
#include "analyticGeometryCompound.h"
#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/ptrHandling.h>

namespace dtOO {
  class vec3dThreeD;
  
  class vec3dThreeDInMap3dTo3d : public map3dTo3d {
  public:
    dt__CLASSSTD(vec3dThreeDInMap3dTo3d, analyticGeometry);
    vec3dThreeDInMap3dTo3d();
    vec3dThreeDInMap3dTo3d(const vec3dThreeDInMap3dTo3d& orig);
    vec3dThreeDInMap3dTo3d(    
	    vec3dThreeD const * const v3d, 
		  map3dTo3d const * const m3d, 
		  bool percentF = false
    );
    virtual ~vec3dThreeDInMap3dTo3d();
    virtual map3dTo3d * clone( void ) const;
    virtual map3dTo3d * create( void ) const;
    virtual dtPoint3 getPoint( float const & uu, float const & vv, float const & ww ) const;
    virtual bool isClosed( int const & dir) const;
    virtual float getMin( int const & dir) const;
    virtual float getMax( int const & dir) const;
  private:
    ptrHandling< vec3dThreeD > _v3d;
    ptrHandling< map3dTo3d > _m3d;
    bool _percentF;
  };
  typedef analyticGeometryCompound<vec3dThreeDInMap3dTo3d> vec3dThreeDInMap3dTo3dCompound;
}
#endif	/* vec3dThreeDInMap3dTo3d_H */

