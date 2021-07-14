#ifndef REPARAMINSURFACE_H
#define	REPARAMINSURFACE_H

#include "dtTransformer.h"
#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>

namespace dtOO {
  class analyticSurface;
  
  class reparamInSurface : public dtTransformer {
    public:    
      dt__classOnlyName(reparamInSurface);
      reparamInSurface();
      virtual ~reparamInSurface();
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
      virtual aGPtrVec apply( 
        aGPtrVec const * const aGeoVecP 
      ) const;
    private:
      dt__pH(analyticSurface const) _aS;
      static bool _registrated;    
  };
}
#endif	/* REPARAMINSURFACE_H */

