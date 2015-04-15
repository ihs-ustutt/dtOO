#ifndef INFINITYMAP3DTO3D_H
#define	INFINITYMAP3DTO3D_H

#include <dtLinearAlgebra.h>
#include "map3dTo3d.h"
#include <logMe/dtMacros.h>
#include <interfaceHeaven/ptrHandling.h>

namespace dtOO {
  class map2dTo3d;
  
  class infinityMap3dTo3d : public map3dTo3d {
  public:
    dt__class(infinityMap3dTo3d, analyticGeometry);
    infinityMap3dTo3d();
    infinityMap3dTo3d(const infinityMap3dTo3d& orig);
    virtual ~infinityMap3dTo3d();
    virtual infinityMap3dTo3d * clone( void ) const;
    virtual infinityMap3dTo3d * create( void ) const;    
    virtual bool isClosed( int const & dir) const;
    virtual float getMin( int const & dir) const;
    virtual float getMax( int const & dir) const;
    virtual dtPoint3 getPoint( float const & uu, float const & vv, float const & ww ) const; 
  };
}
#endif	/* INFINITYMAP3DTO3D_H */

