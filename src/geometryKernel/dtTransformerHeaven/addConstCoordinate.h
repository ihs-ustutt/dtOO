#ifndef ADDCONSTCOORDINATE_H
#define	ADDCONSTCOORDINATE_H

#include <dtOOTypeDef.h>

#include "dtTransformer.h"
#include <logMe/dtMacros.h>

namespace dtOO {
  class analyticFunction;
  class vec2dOneD;
  
  class addConstCoordinate : public dtTransformer{
    public:
      dt__classOnlyName(addConstCoordinate);   
      addConstCoordinate();
      addConstCoordinate(const addConstCoordinate& orig);
			addConstCoordinate( jsonPrimitive const & jE );
      virtual ~addConstCoordinate();
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
      virtual lvH_analyticFunction apply( 
        lvH_analyticFunction const * const aFVecP 
      ) const;
    private: 
      static bool _registrated;      
  };
}

#endif	/* ADDCONSTCOORDINATE_H */

