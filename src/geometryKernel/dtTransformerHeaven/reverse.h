#ifndef reverse_H
#define	reverse_H

#include "dtTransformer.h"
#include <logMe/dtMacros.h>

namespace dtOO {
  class reverse : public dtTransformer {
  public:    
    dt__classOnlyName(reverse);
    reverse();
    virtual ~reverse();
    reverse(const reverse& orig);       
    virtual dtTransformer * clone( void ) const;
    virtual dtTransformer * create( void ) const;     
    virtual bool isNecessary( void ) const;
    void init( 
      ::QDomElement const * tE, 
      baseContainer * const bC,
      cVPtrVec const * const cV,
      aFPtrVec const * const aF,
      aGPtrVec const * const aG 
    );
    virtual aGPtrVec apply( aGPtrVec const * const aGeoVecP ) const;
  private:
    bool _revU;
    bool _revV;
    bool _revW;
  };    
}

#endif	/* reverse_H */
