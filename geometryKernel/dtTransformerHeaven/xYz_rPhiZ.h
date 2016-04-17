#ifndef xYz_rPhiZ_H
#define	xYz_rPhiZ_H

#include "dtTransformer.h"
#include <interfaceHeaven/vectorHandling.h>
#include <logMe/dtMacros.h>

namespace dtOO {
  class xYz_rPhiZ : public dtTransformer {
  public:      
    dt__class(xYz_rPhiZ, dtTransformer);
    xYz_rPhiZ();
    xYz_rPhiZ( xYz_rPhiZ const & orig );
    virtual ~xYz_rPhiZ();
    virtual dtTransformer * clone( void ) const;
    virtual dtTransformer * create( void ) const;     
    virtual bool isNecessary( void ) const;
    void init( 
      ::QDomElement const * tE, 
      baseContainer const * const bC,
      vectorHandling< constValue * > const * const cV,
      vectorHandling< analyticFunction * > const * const aF,
      vectorHandling< analyticGeometry * > const * const aG 
    );
    virtual std::vector< dtPoint3 > 
    apply( std::vector< dtPoint3 > const * const toTrans ) const;        
    std::vector< dtPoint3 > 
    retract(std::vector< dtPoint3 > const * const toRetract) const;    
    dtVector3 const & rotationAxis( void ) const;
    dtVector3 const & referenceAxis( void ) const;
    dtPoint3 const & origin( void ) const;
  private:
    dtVector3 _rotAxis;
    dtVector3 _refAxis;
    dtPoint3 _origin;
  };
}

#endif	/* xYz_rPhiZ_H */

