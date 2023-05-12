#ifndef CLOSEGAPS_H
#define	CLOSEGAPS_H

#include <dtOOTypeDef.h>

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
        baseContainer * const bC,
        lvH_constValue const * const cV,
        lvH_analyticFunction const * const aF,
        lvH_analyticGeometry const * const aG 
      );
      virtual lvH_analyticGeometry apply( 
        lvH_analyticGeometry const * const aGeoVecP 
      ) const;
    private:
      dt__pH(analyticSurface) _vvStartAGeo;
      dt__pH(analyticSurface) _vvEndAGeo;
      static bool _registrated;    
  };
}
#endif	/* CLOSEGAPS_H */

