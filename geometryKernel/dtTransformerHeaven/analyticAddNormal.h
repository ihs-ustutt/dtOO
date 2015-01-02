#ifndef ANALYTICADDNORMAL_H
#define	ANALYTICADDNORMAL_H

#include "dtTransformer.h"
#include <logMe/dtMacros.h>

namespace dtOO {
  class analyticFunction;
  class scaOneD;
  class scaTwoD;
  
  class analyticAddNormal : public dtTransformer {
  public:    
    dt__CLASSNAME(analyticAddNormal);
    analyticAddNormal();
    analyticAddNormal(analyticAddNormal const & orig);
    virtual ~analyticAddNormal();
    virtual dtTransformer * clone( void ) const;
    virtual dtTransformer * create( void ) const;     
    virtual bool isNecessary( void ) const;
    virtual void init( 
      QDomElement const * transformerElementP,
      vectorHandling< constValue * > const * const cValP,
      vectorHandling< analyticFunction * > const * const sFunP
    );
    virtual vectorHandling< analyticFunction * > 
    apply( vectorHandling< analyticFunction * > const * const aFP ) const;
    virtual void handleAnalyticFunction(
      std::string const name, analyticFunction const * value
    );
    virtual void handleBool(std::string const name, bool const value);
  private:
    dt__pH(scaOneD) _s1d_tD0;
    dt__pH(scaOneD) _s1d_tD1;
    dt__pH(scaTwoD) _s2d_tD0;
    dt__pH(scaTwoD) _s2d_tD1;
    bool _reverse;
  };
}    

#endif	/* ANALYTICADDNORMAL_H */

