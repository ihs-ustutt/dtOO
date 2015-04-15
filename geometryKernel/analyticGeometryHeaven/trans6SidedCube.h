#ifndef TRANS6SIDEDCUBE_H
#define	TRANS6SIDEDCUBE_H

#include "map3dTo3d.h"
#include <logMe/dtMacros.h>
#include <interfaceHeaven/ptrHandling.h>

namespace dtOO {
  class map2dTo3d;
  class transIntCube;
  
  class trans6SidedCube : public map3dTo3d {
  public:
    dt__class(trans6SidedCube, analyticGeometry);   
    trans6SidedCube();
    trans6SidedCube(const trans6SidedCube& orig);
    trans6SidedCube(
      map2dTo3d const * const aS0, 
      map2dTo3d const * const aS1,
      map2dTo3d const * const aS2, 
      map2dTo3d const * const aS3, 
      map2dTo3d const * const aS4,
      map2dTo3d const * const aS5
    );    
    virtual trans6SidedCube * clone( void ) const;
    virtual trans6SidedCube * create( void ) const;    
    virtual ~trans6SidedCube();
    virtual bool isClosed( int const & dir) const;
    virtual float getMin( int const & dir) const;
    virtual float getMax( int const & dir) const;
    virtual dtPoint3 getPoint( float const & uu, float const & vv, float const & ww ) const;
  private:
    ptrHandling< transIntCube > _tI;
  };
}
#endif	/* TRANS6SIDEDCUBE_H */

