#ifndef radialTranslate_H
#define	radialTranslate_H

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
      cVPtrVec const * const cV,
		  aFPtrVec const * const aF,
		  aGPtrVec const * const aG 
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
    virtual aGPtrVec apply( aGPtrVec const * const aGeoVecP ) const;
  private:
    dtVector3 _v3;
    float _distance;
  };    
}

#endif	/* radialTranslate_H */

