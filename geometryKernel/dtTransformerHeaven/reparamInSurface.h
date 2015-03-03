#ifndef REPARAMINSURFACE_H
#define	REPARAMINSURFACE_H

#include "dtTransformer.h"
#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>

namespace dtOO {
  class analyticSurface;
  
  class reparamInSurface : public dtTransformer {
  public:    
    dt__CLASSNAME(reparamInSurface);
    reparamInSurface();
    virtual ~reparamInSurface();
    virtual dtTransformer * clone( void ) const;
    virtual dtTransformer * create( void ) const;     
    virtual bool isNecessary( void ) const;
    void init( 
      QDomElement const * tE, 
      baseContainer const * const bC,
      vectorHandling< constValue * > const * const cV,
      vectorHandling< analyticFunction * > const * const aF,
      vectorHandling< analyticGeometry * > const * const aG 
    );
    virtual vectorHandling< analyticGeometry * > apply( vectorHandling< analyticGeometry * > const * const aGeoVecP ) const;
  private:
    dt__pH(analyticSurface const) _aS;
  };
}
#endif	/* REPARAMINSURFACE_H */

