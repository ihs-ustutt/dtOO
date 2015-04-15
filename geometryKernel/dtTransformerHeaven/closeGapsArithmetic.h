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
      QDomElement const * tE, 
      baseContainer const * const bC,
      vectorHandling< constValue * > const * const cV,
      vectorHandling< analyticFunction * > const * const aF,
      vectorHandling< analyticGeometry * > const * const aG 
    );
    virtual vectorHandling< analyticGeometry * > apply( vectorHandling< analyticGeometry * > const * const aGeoVecP ) const;
  private:
  };
}
#endif	/* closeGapsArithmetic_H */

