#ifndef map1dTo3dInMap3dTo3d_H
#define	map1dTo3dInMap3dTo3d_H

#include "map1dTo3d.h"
#include "map3dTo3d.h"
#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/ptrHandling.h>

namespace dtOO {
  class map1dTo3d;
  class map3dTo3d;
  
  class map1dTo3dInMap3dTo3d : public map1dTo3d {
  public:
    dt__CLASSSTD(map1dTo3dInMap3dTo3d, analyticGeometry);    
    map1dTo3dInMap3dTo3d();
    map1dTo3dInMap3dTo3d(const map1dTo3dInMap3dTo3d& orig);
    map1dTo3dInMap3dTo3d(
      map1dTo3d const * const m1d,
      map3dTo3d const * const m3d
    );
    virtual ~map1dTo3dInMap3dTo3d();
    virtual map1dTo3dInMap3dTo3d * clone( void ) const;
    virtual map1dTo3dInMap3dTo3d * create( void ) const;    
    virtual dtPoint3 getPoint( float const & uu ) const;    
    virtual bool isClosed( int const & dir) const; 
    virtual float getMin( int const & dir) const;
    virtual float getMax( int const & dir) const;   
    map3dTo3d const * getConstPtrToMap3dTo3d( void ) const;
    map1dTo3d const * getConstPtrToMap1dTo3d( void ) const;
  private:
    ptrHandling< map1dTo3d > _m1d;  
    ptrHandling< map3dTo3d > _m3d;  
  };
}
#endif	/* map1dTo3dInMap3dTo3d_H */

