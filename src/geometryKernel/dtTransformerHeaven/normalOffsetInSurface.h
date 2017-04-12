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
      baseContainer * const bC,
      cVPtrVec const * const cV,
      aFPtrVec const * const aF,
      aGPtrVec const * const aG 
    );
    virtual aGPtrVec apply( aGPtrVec const * const aGeoVecP ) const;
  private:
    dt__pH(map2dTo3d const) _m2d;
    float _thickness;
    int _nPoints;
    int _nIntPoints;
    int _order;
  };
}
#endif	/* normalOffsetInSurface_H */

