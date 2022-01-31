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
        cVPtrVec const * const cV,
        aFPtrVec const * const aF,
        aGPtrVec const * const aG 
      );
      virtual aGPtrVec apply( aGPtrVec const * const aGeoVecP ) const;
    private:
      dtInt _nPointsOne;
      dtInt _nPointsTwo;
      dtInt _orderOne;
      dtInt _orderTwo;
      static bool _registrated;    
  };
}
#endif	/* MAKEPOLYNOMIAL_H */

