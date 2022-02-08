#ifndef approxInSurface_H
#define	approxInSurface_H

#include <dtOOTypeDef.h>

#include "dtTransformer.h"
#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>

namespace dtOO {
  class map2dTo3d;
  
  class approxInSurface : public dtTransformer {
    public:    
      dt__classOnlyName(approxInSurface);
      approxInSurface();
      approxInSurface( approxInSurface const & orig );    
      virtual ~approxInSurface();
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
      dtInt _nPoints;
      dtInt _order;
      static bool _registrated;    
  };
}
#endif	/* approxInSurface_H */

