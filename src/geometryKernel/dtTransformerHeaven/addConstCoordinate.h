#ifndef ADDCONSTCOORDINATE_H
#define	ADDCONSTCOORDINATE_H

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
        cVPtrVec const * const cV,
        aFPtrVec const * const aF,
        aGPtrVec const * const aG 
      );
      virtual aFPtrVec apply( 
        aFPtrVec const * const aFVecP 
      ) const;
      virtual void handleFloat(const std::string name, const float value);
      virtual void handleDtVector3(const std::string name, const dtVector3 value);
    private:
      float _cc;
      dtVector3 _vv;
    private: 
      static bool _registrated;      
  };
}

#endif	/* ADDCONSTCOORDINATE_H */

