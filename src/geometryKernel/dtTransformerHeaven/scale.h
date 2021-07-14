#ifndef scale_H
#define	scale_H

#include "dtTransformerInvThreeD.h"
#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>

namespace dtOO {
  class scale : public dtTransformerInvThreeD {
    public:    
      dt__classOnlyName(scale);
      scale();
      virtual ~scale();
      scale(const scale& orig);    
      virtual dtTransformerInvThreeD * clone( void ) const;
      virtual dtTransformerInvThreeD * create( void ) const;       
      virtual bool isNecessary( void ) const;
      void init(
        ::QDomElement const * tE, 
        baseContainer * const bC,
        cVPtrVec const * const cV,
        aFPtrVec const * const aF,
        aGPtrVec const * const aG 
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
    private:
      dtVector3 _scale;
      static bool _registrated;    
  };
}
#endif	/* scale_H */

