#ifndef xYz_rPhiZ_H
#define	xYz_rPhiZ_H

#include <dtOOTypeDef.h>

#include "dtTransformer.h"
#include <interfaceHeaven/vectorHandling.h>
#include <logMe/dtMacros.h>

namespace dtOO {
  class xYz_rPhiZ : public dtTransformer {
    public:      
      dt__class(xYz_rPhiZ, dtTransformer);
      xYz_rPhiZ();
      xYz_rPhiZ( xYz_rPhiZ const & orig );
      xYz_rPhiZ( jsonPrimitive const & jE );
      virtual ~xYz_rPhiZ();
      virtual dtTransformer * clone( void ) const;
      virtual dtTransformer * create( void ) const;     
      virtual bool isNecessary( void ) const;
      virtual void jInit( 
        jsonPrimitive const & jE, 
        baseContainer * const bC,
        lvH_constValue const * const cV,
        lvH_analyticFunction const * const aF,
        lvH_analyticGeometry const * const aG
      );              
      void init( 
        ::QDomElement const * tE, 
        baseContainer * const bC,
        lvH_constValue const * const cV,
        lvH_analyticFunction const * const aF,
        lvH_analyticGeometry const * const aG 
      );
      virtual std::vector< dtPoint3 > 
      apply( std::vector< dtPoint3 > const * const toTrans ) const;        
      std::vector< dtPoint3 > 
      retract(std::vector< dtPoint3 > const * const toRetract) const;    
      dtVector3 const rotationAxis( void ) const;
      dtVector3 const referenceAxis( void ) const;
      dtPoint3 const origin( void ) const;
    private:
      static bool _registrated;    
  };
}

#endif	/* xYz_rPhiZ_H */

