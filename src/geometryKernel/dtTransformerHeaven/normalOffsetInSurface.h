#ifndef normalOffsetInSurface_H
#define	normalOffsetInSurface_H

#include <dtOOTypeDef.h>

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
        lvH_constValue const * const cV,
        lvH_analyticFunction const * const aF,
        lvH_analyticGeometry const * const aG 
      );
      virtual lvH_analyticGeometry apply( lvH_analyticGeometry const * const aGeoVecP ) const;
    private:
      dt__pH(map2dTo3d const) _m2d;
      dtReal _thickness;
      dtInt _nPoints;
      dtInt _nIntPoints;
      dtInt _order;
      static bool _registrated;    
  };
}
#endif	/* normalOffsetInSurface_H */

