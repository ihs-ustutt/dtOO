#ifndef radialTranslate_H
#define	radialTranslate_H

#include <dtOOTypeDef.h>

#include "dtTransformerInvThreeD.h"
#include <logMe/dtMacros.h>

namespace dtOO {
  class radialTranslate : public dtTransformerInvThreeD {
    public:    
      dt__class(radialTranslate, dtTransformerInvThreeD);
      radialTranslate();
      radialTranslate( radialTranslate const & orig );
      virtual ~radialTranslate();
      virtual dtTransformerInvThreeD * clone( void ) const;
      virtual dtTransformerInvThreeD * create( void ) const;     
      virtual bool isNecessary( void ) const;
      virtual void init(
        ::QDomElement const * tE, 
        baseContainer * const bC,
        lvH_constValue const * const cV,
        lvH_analyticFunction const * const aF,
        lvH_analyticGeometry const * const aG 
      ); 
      virtual std::vector< dtPoint3 > apply( 
        std::vector< dtPoint3 > const * const toTrans 
      ) const;
      virtual std::vector< dtVector3 > apply( 
        std::vector< dtVector3 > const * const toTrans 
      ) const;    
      virtual std::vector< dtPoint3 > retract( 
        std::vector< dtPoint3 > const * const toTrans 
      ) const;
      virtual std::vector< dtVector3 > retract( 
        std::vector< dtVector3 > const * const toTrans 
      ) const;        
      virtual lvH_analyticGeometry apply( lvH_analyticGeometry const * const aGeoVecP ) const;
    private:
      dtVector3 _v3;
      dtReal _distance;
      static bool _registrated;    
  };    
}

#endif	/* radialTranslate_H */

