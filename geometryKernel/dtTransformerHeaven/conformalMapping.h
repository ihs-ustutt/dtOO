#ifndef conformalMapping_H
#define	conformalMapping_H

#include "dtTransformer.h"
#include <vector>
#include <logMe/dtMacros.h>

namespace dtOO {
  class rotatingSpline;
  
  class conformalMapping : public dtTransformer {
  public:      
    dt__CLASSNAME(conformalMapping);
    conformalMapping();
    conformalMapping( conformalMapping const & orig );
    virtual ~conformalMapping();
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
    virtual std::vector< dtPoint2 * > apply( std::vector< dtPoint2 * > const * const pointVecP ) const;
    virtual void handleFloat(std::string const name, float const value);
    virtual void handleAnalyticGeometry(std::string const name, analyticGeometry const * value);
  protected:
    float _tolerance;
    rotatingSpline const * _rotSplineP;
  };
}

#endif	/* conformalMapping_H */

