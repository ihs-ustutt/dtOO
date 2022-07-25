#ifndef reverse_H
#define	reverse_H

#include <dtOOTypeDef.h>

#include "dtTransformer.h"
#include <logMe/dtMacros.h>

namespace dtOO {
  class reverse : public dtTransformer {
    public:    
      dt__classOnlyName(reverse);
      reverse();
      virtual ~reverse();
      reverse(const reverse& orig);   
      reverse( jsonPrimitive const & jE );
      virtual dtTransformer * clone( void ) const;
      virtual dtTransformer * create( void ) const;     
      virtual bool isNecessary( void ) const;
      virtual void jInit( 
        jsonPrimitive const & jE, 
        baseContainer * const bC,
        cVPtrVec const * const cV,
        aFPtrVec const * const aF,
        aGPtrVec const * const aG
      );      
      void init( 
        ::QDomElement const * tE, 
        baseContainer * const bC,
        cVPtrVec const * const cV,
        aFPtrVec const * const aF,
        aGPtrVec const * const aG 
      );
      virtual aGPtrVec apply( aGPtrVec const * const aGeoVecP ) const;
      virtual aFPtrVec apply( aFPtrVec const * const aF ) const;
    private:
      static bool _registrated;    
  };    
}

#endif	/* reverse_H */

