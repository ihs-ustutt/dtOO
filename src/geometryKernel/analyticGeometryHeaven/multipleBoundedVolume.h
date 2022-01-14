#ifndef multipleBoundedVolume_H
#define	multipleBoundedVolume_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include "analyticGeometry.h"
#include <logMe/dtMacros.h>
#include <interfaceHeaven/ptrHandling.h>

namespace dtOO {
  class multipleBoundedVolume : public analyticGeometry {
  public:
    dt__class(multipleBoundedVolume, analyticGeometry);
    multipleBoundedVolume();
    multipleBoundedVolume(const multipleBoundedVolume& orig);
    virtual ~multipleBoundedVolume();
    multipleBoundedVolume( 
      analyticGeometry const * const m3d, 
      ptrVectorHandling< analyticGeometry > const & m2d
    );
    
    virtual multipleBoundedVolume * create( void ) const;        
    virtual multipleBoundedVolume * clone( void ) const;
    virtual int dim( void ) const;    
    virtual bool isClosed( int const & dir) const;
    virtual dtReal getMin( int const & dir) const;
    virtual dtReal getMax( int const & dir) const;
    virtual dtPoint3 getPoint( dtReal const * const uvw ) const; 
    vectorHandling< renderInterface * > getRender( void ) const; 
    ptrVectorHandling< analyticGeometry > const & 
    boundsVectorConstRef( void ) const;
    analyticGeometry const * const regionConstPtr( void ) const;    
  private:
    ptrHandling< analyticGeometry > _m3d;
    ptrVectorHandling< analyticGeometry > _m2d;
  };
}
#endif	/* multipleBoundedVolume_H */

