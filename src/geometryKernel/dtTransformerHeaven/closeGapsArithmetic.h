#ifndef closeGapsArithmetic_H
#define	closeGapsArithmetic_H

#include "dtTransformer.h"
#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>

namespace dtOO {
  class map2dTo3d;
  
  class closeGapsArithmetic : public dtTransformer {
  public:    
    dt__classOnlyName(closeGapsArithmetic);
    closeGapsArithmetic();
    closeGapsArithmetic( closeGapsArithmetic const & orig );    
    virtual ~closeGapsArithmetic();
    virtual dtTransformer * clone( void ) const;
    virtual dtTransformer * create( void ) const;     
    virtual bool isNecessary( void ) const;
    void init( 
      ::QDomElement const * tE, 
      baseContainer const * const bC,
      cVPtrVec const * const cV,
      aFPtrVec const * const aF,
      aGPtrVec const * const aG 
    );
    virtual aGPtrVec apply( 
      aGPtrVec const * const aGeoVecP 
    ) const;
    virtual aFPtrVec apply( 
      aFPtrVec const * const aF 
    ) const;    
  private:
    int _dir;
    int _nSegments;
  };
}
#endif	/* closeGapsArithmetic_H */

