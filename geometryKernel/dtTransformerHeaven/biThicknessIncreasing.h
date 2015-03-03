#ifndef BITHICKNESSINCREASING_H
#define	BITHICKNESSINCREASING_H

#include "dtTransformer.h"
#include <logMe/dtMacros.h>

namespace dtOO {
  class analyticFunction;
  class scaOneD;
  
  class biThicknessIncreasing : public dtTransformer {
  public:    
    dt__CLASSNAME(biThicknessIncreasing);
    biThicknessIncreasing();
    virtual ~biThicknessIncreasing();
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
    virtual vectorHandling< analyticFunction * > 
    apply( vectorHandling< analyticFunction * > const * const sFunP ) const;  
    virtual void handleInt(std::string const name, int const value);
    virtual void handleAnalyticFunction(
      std::string const name, analyticFunction const * value
    );
    virtual void handleBool(std::string const name, bool const value);
  private:
    scaOneD const * _tD[2];
    scaOneD const * _para;
    int _nPointsOne;
    int _splineOrder;
    bool _reverse;
  };
}    

#endif	/* THICKNESSINCREASING_H */

