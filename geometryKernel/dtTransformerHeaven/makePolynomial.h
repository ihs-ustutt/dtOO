#ifndef MAKEPOLYNOMIAL_H
#define	MAKEPOLYNOMIAL_H

#include "dtTransformer.h"
#include <logMe/dtMacros.h>
#include <dtLinearAlgebra.h>

namespace dtOO {
  class analyticGeometry;
  
  class makePolynomial : public dtTransformer {
  public:
    dt__CLASSNAME(makePolynomial);
    makePolynomial();
    makePolynomial(const makePolynomial& orig);
    virtual ~makePolynomial();
    virtual dtTransformer * clone( void ) const;
    virtual dtTransformer * create( void ) const;     
    virtual bool isNecessary( void ) const;
    void init( QDomElement * transformerElementP, 
               pointContainer * const pointContainerP,
               vectorContainer * const vectorContainerP,    
               vectorHandling< constValue * > const * const cValP,
               vectorHandling< analyticFunction * > const * const sFunP,
               vectorHandling< analyticGeometry * > const * const depAGeoP );
    virtual vectorHandling< analyticGeometry * > apply( vectorHandling< analyticGeometry * > const * const aGeoVecP ) const;
  private:
    int _nPointsOne;
    int _nPointsTwo;
    int _orderOne;
    int _orderTwo;
  };
}
#endif	/* MAKEPOLYNOMIAL_H */

