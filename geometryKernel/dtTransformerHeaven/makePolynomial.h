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
      QDomElement const * tE, 
      baseContainer const * const bC,
      vectorHandling< constValue * > const * const cV,
      vectorHandling< analyticFunction * > const * const aF,
      vectorHandling< analyticGeometry * > const * const aG 
    );
    virtual vectorHandling< analyticGeometry * > apply( vectorHandling< analyticGeometry * > const * const aGeoVecP ) const;
  private:
    int _nPointsOne;
    int _nPointsTwo;
    int _orderOne;
    int _orderTwo;
  };
}
#endif	/* MAKEPOLYNOMIAL_H */

