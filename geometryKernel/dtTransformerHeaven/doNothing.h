#ifndef DONOTHING_H
#define	DONOTHING_H

#include "dtTransformer.h"
#include <logMe/dtMacros.h>

namespace dtOO {
  class doNothing : public dtTransformer {
  public:
    dt__classOnlyName(doNothing);
    doNothing();
    virtual ~doNothing();
    virtual dtTransformer * clone( void ) const;
    virtual dtTransformer * create( void ) const;     
    virtual bool isNecessary( void ) const;
    virtual void init( 
      QDomElement const * tE, 
      baseContainer const * const bC,
      vectorHandling< constValue * > const * const cValP,
      vectorHandling< analyticFunction * > const * const sFunP,
      vectorHandling< analyticGeometry * > const * const depAGeoP 
    );    
    virtual std::vector< dtPoint2 * > apply( std::vector< dtPoint2 * > const * const pointVecP ) const;    
    virtual std::vector< dtPoint3 * > apply( std::vector< dtPoint3 * > const * const pointVecP ) const;    
    virtual vectorHandling< analyticGeometry * > apply( vectorHandling< analyticGeometry * > const * const aGeoVecP ) const;  
    virtual vectorHandling< analyticFunction * > apply( vectorHandling< analyticFunction * > const * const sFunVecP ) const;  
  private:

  };
}

#endif	/* DONOTHING_H */

