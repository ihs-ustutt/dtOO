#ifndef ANALYTICADDNORMAL_H
#define	ANALYTICADDNORMAL_H

#include "dtTransformer.h"
#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>

namespace dtOO {
  class analyticFunction;
  class vec3dTwoD;
  
  class analyticAddNormal : public dtTransformer {
  public:    
    dt__classOnlyName(analyticAddNormal);
    analyticAddNormal();
    analyticAddNormal(analyticAddNormal const & orig);
    virtual ~analyticAddNormal();
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
    apply( vectorHandling< analyticFunction * > const * const aFP ) const;
    virtual void handleAnalyticFunction(
      std::string const name, analyticFunction const * value
    );
    virtual void handleDtVector3(std::string const name, dtVector3 const value);
  private:
    dt__pH(vec3dTwoD) _tt;
    dtVector3 _nf;
  };
}    

#endif	/* ANALYTICADDNORMAL_H */

