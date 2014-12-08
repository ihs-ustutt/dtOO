#ifndef MAP1DTO3D_H
#define	MAP1DTO3D_H

#include <dtLinearAlgebra.h>
#include "analyticGeometry.h"
#include <logMe/dtMacros.h>

namespace dtOO {
  class map1dTo3d : public analyticGeometry {
  public:
    dt__CLASSSTD(map1dTo3d, analyticGeometry);
    map1dTo3d();
    map1dTo3d(const map1dTo3d& orig);
    virtual ~map1dTo3d();
    virtual map1dTo3d * clone( void ) const = 0;
    virtual map1dTo3d * create( void ) const = 0;    
    virtual dtPoint3 getPoint( float const & uu ) const = 0;
    dtPoint3 getPointPercent( float const & uu ) const;  
    int getRenderResolutionU( void ) const;
    float u_percent(float const & uu) const;
    float percent_u(float const & uu) const;
    virtual bool isClosed( int const & dir) const = 0;
    virtual float getMin( int const & dir) const = 0;
    virtual float getMax( int const & dir) const = 0;    
    virtual bool isClosedU( void ) const;
    virtual float getUMin( void ) const;
    virtual float getUMax( void ) const;
    virtual dtVector3 firstDerU( float const & uu) const;
    virtual std::vector< dtVector3 > firstDerUOrder( int const & nU ) const;
    virtual float l_u( float const & uu ) const;
    vectorHandling< renderInterface * > getRender( void ) const;
  private:
    float l_u( float const & uu, int const & nP ) const;    
  };
}
#endif	/* MAP1DTO3D_H */

