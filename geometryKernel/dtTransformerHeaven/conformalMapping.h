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
    virtual ~conformalMapping();
    virtual bool isNecessary( void ) const;
    void init( QDomElement * transformerElementP, 
               pointContainer * const pointContainerP,
               vectorContainer * const vectorContainerP,    
               vectorHandling< constValue * > const * const cValP,
               vectorHandling< analyticFunction * > const * const sFunP,
               vectorHandling< analyticGeometry * > const * const depAGeoP );
    virtual std::vector< dtPoint2 * > apply( std::vector< dtPoint2 * > const * const pointVecP ) const;
    virtual void handleFloat(std::string const name, float const value);
    virtual void handleAnalyticGeometry(std::string const name, analyticGeometry const * value);
  protected:
    float _tolerance;
    rotatingSpline const * _rotSplineP;
  };
}

#endif	/* conformalMapping_H */

