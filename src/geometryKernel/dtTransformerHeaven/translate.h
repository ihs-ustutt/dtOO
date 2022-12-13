#ifndef translate_H
#define	translate_H

#include <dtOOTypeDef.h>

#include "dtTransformer.h"
#include <logMe/dtMacros.h>

namespace dtOO {
  class translate : public dtTransformer {
    public:    
      dt__class(translate, dtTransformer);
      translate();
      translate( translate const & orig );
			translate( jsonPrimitive const & jE );
      virtual ~translate();
      virtual dtTransformer * clone( void ) const;
      virtual dtTransformer * create( void ) const;     
      virtual bool isNecessary( void ) const;
    	void jInit( 
        jsonPrimitive const & jE,
        baseContainer * const bC,
    		lvH_constValue const * const cV,
    		lvH_analyticFunction const * const aF,
    		lvH_analyticGeometry const * const aG 
    	);
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
      virtual lvH_analyticFunction apply( 
			  lvH_analyticFunction const * const sFunP 
			) const; 
      virtual lvH_analyticGeometry apply( 
			  lvH_analyticGeometry const * const aGeoVecP 
			) const;
      std::vector< dtPoint3 > 
      retract(std::vector< dtPoint3 > const * const toRetract) const;       
      dtVector3 translationAxis3d( void ) const;
      dtVector2 translationAxis2d( void ) const;
    private:
      static bool _registrated;    
  };    
}

#endif	/* translate_H */

