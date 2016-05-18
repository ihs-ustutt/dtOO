#ifndef CLOSEGAPS_H
#define	CLOSEGAPS_H

#include "dtTransformer.h"
#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>

namespace dtOO {
  class analyticSurface;
  
  class closeGaps : public dtTransformer {
  public:    
    dt__classOnlyName(closeGaps);
    closeGaps();
    virtual ~closeGaps();
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
    virtual vectorHandling< analyticGeometry * > apply( 
      vectorHandling< analyticGeometry * > const * const aGeoVecP 
    ) const;
  private:
    dt__pH(analyticSurface) _vvStartAGeo;
    dt__pH(analyticSurface) _vvEndAGeo;
  };
}
#endif	/* CLOSEGAPS_H */

