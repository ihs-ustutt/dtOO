#ifndef multipleBoundedSurface_H
#define	multipleBoundedSurface_H

#include <dtLinearAlgebra.h>
#include "analyticGeometry.h"
#include <logMe/dtMacros.h>
#include <interfaceHeaven/ptrHandling.h>

namespace dtOO {
  class multipleBoundedSurface : public analyticGeometry {
  public:
    dt__class(multipleBoundedSurface, analyticGeometry);
    multipleBoundedSurface();
    multipleBoundedSurface(const multipleBoundedSurface& orig);
    virtual ~multipleBoundedSurface();
    multipleBoundedSurface( 
      analyticGeometry const * const m2d, 
      ptrVectorHandling< analyticGeometry > const & m1d
    );
    
    virtual multipleBoundedSurface * create( void ) const;        
    virtual multipleBoundedSurface * clone( void ) const;
    virtual int dim( void ) const;    
    virtual bool isClosed( int const & dir) const;
    virtual float getMin( int const & dir) const;
    virtual float getMax( int const & dir) const;
    virtual dtPoint3 getPoint( float const * const uvw ) const; 
    vectorHandling< renderInterface * > getRender( void ) const;
		vectorHandling< renderInterface * > rV;    
  private:
    ptrHandling< analyticGeometry > _m2d;
    ptrVectorHandling< analyticGeometry > _m1d;
  };
}
#endif	/* multipleBoundedSurface_H */

