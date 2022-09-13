#ifndef REPARAMINSURFACE_H
#define	REPARAMINSURFACE_H

#include <dtOOTypeDef.h>

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
        lvH_constValue const * const cV,
        lvH_analyticFunction const * const aF,
        lvH_analyticGeometry const * const aG 
      );
      virtual lvH_analyticGeometry apply( 
        lvH_analyticGeometry const * const aGeoVecP 
      ) const;
    private:
      dt__pH(analyticSurface const) _aS;
      static bool _registrated;    
  };
}
#endif	/* REPARAMINSURFACE_H */

