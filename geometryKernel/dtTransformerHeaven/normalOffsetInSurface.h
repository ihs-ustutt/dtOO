#ifndef normalOffsetInSurface_H
#define	normalOffsetInSurface_H

#include "dtTransformer.h"
#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>

namespace dtOO {
  class map2dTo3d;
  
  class normalOffsetInSurface : public dtTransformer {
  public:    
    dt__classOnlyName(normalOffsetInSurface);
    normalOffsetInSurface();
    normalOffsetInSurface( normalOffsetInSurface const & orig );    
    virtual ~normalOffsetInSurface();
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
    virtual vectorHandling< analyticGeometry * > apply( vectorHandling< analyticGeometry * > const * const aGeoVecP ) const;
  private:
    dt__pH(map2dTo3d const) _m2d;
    float _thickness;
    int _nPoints;
    int _nIntPoints;
    int _order;
  };
}
#endif	/* normalOffsetInSurface_H */

