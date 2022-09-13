#ifndef ROTATE_H
#define	ROTATE_H

#include <dtOOTypeDef.h>

#include "dtTransformerInvThreeD.h"
#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>

namespace dtOO {
  class rotate : public dtTransformerInvThreeD {
    public:    
      dt__classOnlyName(rotate);
      rotate();
      virtual ~rotate();
      rotate(const rotate& orig);    
      rotate( jsonPrimitive const & jE );
      virtual dtTransformerInvThreeD * clone( void ) const;
      virtual dtTransformerInvThreeD * create( void ) const;       
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
      virtual std::vector< dtPoint3 > apply( 
        std::vector< dtPoint3 > const * const toTrans 
      ) const;
      virtual std::vector< dtPoint3 > retract(
        std::vector< dtPoint3 > const * const toRetract
      ) const;
      virtual std::vector< dtVector3 > apply( 
        std::vector< dtVector3 > const * const toTrans 
      ) const;
      virtual std::vector< dtVector3 > retract(
        std::vector< dtVector3 > const * const toRetract
      ) const;        
      virtual lvH_analyticGeometry apply( 
        lvH_analyticGeometry const * const aGeoVecP 
      ) const;
    private:
      static bool _registrated;    
  };
}
#endif	/* ROTATE_H */

