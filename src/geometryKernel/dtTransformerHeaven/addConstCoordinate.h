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
      virtual ~addConstCoordinate();
      virtual dtTransformer * clone( void ) const;
      virtual dtTransformer * create( void ) const;     
      virtual bool isNecessary( void ) const; 
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
      virtual void handleFloat(const std::string name, const dtReal value);
      virtual void handleDtVector3(const std::string name, const dtVector3 value);
    private:
      dtReal _cc;
      dtVector3 _vv;
    private: 
      static bool _registrated;      
  };
}

#endif	/* ADDCONSTCOORDINATE_H */

