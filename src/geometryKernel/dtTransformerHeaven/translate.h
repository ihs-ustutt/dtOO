#ifndef TRANSLATE_H
#define	TRANSLATE_H

#include "dtTransformer.h"
#include <logMe/dtMacros.h>

namespace dtOO {
  class translate : public dtTransformer {
  public:    
    dt__classOnlyName(translate);
    translate();
    translate( translate const & orig );
    virtual ~translate();
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
    virtual std::vector< dtPoint3 > apply( 
      std::vector< dtPoint3 > const * const toTrans 
    ) const;   
    virtual aFPtrVec apply( 
      aFPtrVec const * const sFunP 
    ) const; 
  private:
    dtVector2 _v2;
    dtVector3 _v3;
  };    
}

#endif	/* TRANSLATE_H */

