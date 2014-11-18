#ifndef CLOSEGAPS_H
#define	CLOSEGAPS_H

#include "dtTransformer.h"
#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>

namespace dtOO {
  class analyticSurface;
  
  class closeGaps : public dtTransformer {
  public:    
    dt__CLASSNAME(closeGaps);
    closeGaps();
    virtual ~closeGaps();
    virtual dtTransformer * clone( void ) const;
    virtual dtTransformer * create( void ) const;     
    virtual bool isNecessary( void ) const;
    void init( 
      QDomElement * transformerElementP, 
      baseContainer * const bC, 
      vectorHandling< constValue * > const * const cValP,
      vectorHandling< analyticFunction * > const * const sFunP,
      vectorHandling< analyticGeometry * > const * const depAGeoP 
    );
    virtual vectorHandling< analyticGeometry * > apply( vectorHandling< analyticGeometry * > const * const aGeoVecP ) const;
  private:
    analyticSurface const * _vvStartAGeo;
    analyticSurface const * _vvEndAGeo;
  };
}
#endif	/* CLOSEGAPS_H */

