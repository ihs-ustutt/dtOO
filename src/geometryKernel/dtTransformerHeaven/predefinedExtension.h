#ifndef PREDEFINEDEXTENSION_H
#define	PREDEFINEDEXTENSION_H

#include "dtTransformer.h"
#include <logMe/dtMacros.h>

namespace dtOO {
  class analyticFunction;

  class predefinedExtension : public dtTransformer {
    public:    
      dt__classOnlyName(predefinedExtension);
      predefinedExtension();
      predefinedExtension( predefinedExtension const & orig );
      virtual ~predefinedExtension();
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
      virtual aFPtrVec apply( aFPtrVec const * const sFunP ) const;  
    private:
      std::vector< dtPoint2 > calculateExtPoints(
        dtPoint2 const & start, dtVector2 const & vvStart,
        dtPoint2 const & end, dtVector2 const & vvEnd    
      ) const;
    private:
      float _yIn;
      float _yOut;
      float _alphaIn;
      float _alphaOut;    
      static bool _registrated;    
  };
}    

#endif	/* PREDEFINEDEXTENSION_H */

