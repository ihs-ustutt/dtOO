#ifndef MAKEPOLYNOMIAL_H
#define	MAKEPOLYNOMIAL_H

#include "dtTransformer.h"
#include <logMe/dtMacros.h>
#include <dtLinearAlgebra.h>

namespace dtOO {
  class analyticGeometry;
  
  class makePolynomial : public dtTransformer {
  public:
    dt__classOnlyName(makePolynomial);
    makePolynomial();
    makePolynomial(const makePolynomial& orig);
    virtual ~makePolynomial();
    virtual dtTransformer * clone( void ) const;
    virtual dtTransformer * create( void ) const;     
    virtual bool isNecessary( void ) const;
    virtual void init( 
      ::QDomElement const * tE, 
      baseContainer const * const bC,
      cVPtrVec const * const cV,
      aFPtrVec const * const aF,
      aGPtrVec const * const aG 
    );
    virtual aGPtrVec apply( aGPtrVec const * const aGeoVecP ) const;
  private:
    int _nPointsOne;
    int _nPointsTwo;
    int _orderOne;
    int _orderTwo;
  };
}
#endif	/* MAKEPOLYNOMIAL_H */

