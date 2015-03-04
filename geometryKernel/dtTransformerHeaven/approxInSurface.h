#ifndef approxInSurface_H
#define	approxInSurface_H

#include "dtTransformer.h"
#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>

namespace dtOO {
  class map2dTo3d;
  
  class approxInSurface : public dtTransformer {
  public:    
    dt__CLASSNAME(approxInSurface);
    approxInSurface();
    approxInSurface( approxInSurface const & orig );    
    virtual ~approxInSurface();
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
    dt__pH(map2dTo3d const) _m2d;
    int _nPoints;
    int _order;
  };
}
#endif	/* approxInSurface_H */

