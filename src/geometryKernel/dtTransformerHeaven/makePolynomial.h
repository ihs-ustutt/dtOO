#ifndef MAKEPOLYNOMIAL_H
#define	MAKEPOLYNOMIAL_H

#include <dtOOTypeDef.h>

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
        baseContainer * const bC,
        lvH_constValue const * const cV,
        lvH_analyticFunction const * const aF,
        lvH_analyticGeometry const * const aG 
      );
      virtual lvH_analyticGeometry apply( lvH_analyticGeometry const * const aGeoVecP ) const;
    private:
      dtInt _nPointsOne;
      dtInt _nPointsTwo;
      dtInt _orderOne;
      dtInt _orderTwo;
      static bool _registrated;    
  };
}
#endif	/* MAKEPOLYNOMIAL_H */

