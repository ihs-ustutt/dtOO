#ifndef closeGapsArithmetic_H
#define	closeGapsArithmetic_H

#include <dtOOTypeDef.h>

#include "dtTransformer.h"
#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>

namespace dtOO {
  class map2dTo3d;
  
  class closeGapsArithmetic : public dtTransformer {
    public:    
      dt__classOnlyName(closeGapsArithmetic);
      closeGapsArithmetic();
      closeGapsArithmetic( closeGapsArithmetic const & orig );    
      virtual ~closeGapsArithmetic();
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
      virtual lvH_analyticFunction apply( 
        lvH_analyticFunction const * const aF 
      ) const;    
    private:
      dtInt _dir;
      dtInt _nSegments;
      static bool _registrated;    
  };
}
#endif	/* closeGapsArithmetic_H */

