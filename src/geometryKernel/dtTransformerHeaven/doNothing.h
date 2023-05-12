#ifndef DONOTHING_H
#define	DONOTHING_H

#include <dtOOTypeDef.h>

#include "dtTransformer.h"
#include <logMe/dtMacros.h>

namespace dtOO {
  class doNothing : public dtTransformer {
    public:
      dt__classOnlyName(doNothing);
      doNothing();
      virtual ~doNothing();
      virtual dtTransformer * clone( void ) const;
      virtual dtTransformer * create( void ) const;     
      virtual bool isNecessary( void ) const;
      virtual void init( 
        ::QDomElement const * tE, 
        baseContainer * const bC,
        lvH_constValue const * const cValP,
        lvH_analyticFunction const * const sFunP,
        lvH_analyticGeometry const * const depAGeoP 
      );    
      virtual std::vector< dtPoint2 * > apply( 
        std::vector< dtPoint2 * > const * const pointVecP 
      ) const;    
      virtual std::vector< dtPoint3 * > apply( 
        std::vector< dtPoint3 * > const * const pointVecP 
      ) const;    
      virtual lvH_analyticGeometry apply( lvH_analyticGeometry const * const aGeoVecP ) const;  
      virtual lvH_analyticFunction apply( lvH_analyticFunction const * const sFunVecP ) const;  
    private: 
      static bool _registrated;

  };
}

#endif	/* DONOTHING_H */

