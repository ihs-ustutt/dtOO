#ifndef PICKRANGE_H
#define	PICKRANGE_H

#include "dtTransformer.h"
#include <logMe/dtMacros.h>

namespace dtOO {
  class analyticFunction;
  class vec2dOneD;
  
  class pickRange : public dtTransformer{
  public:
    dt__CLASSNAME(pickRange);   
    pickRange();
    pickRange(const pickRange& orig);
    virtual ~pickRange();
    virtual bool isNecessary( void ) const; 
    void init( QDomElement * transformerElementP, 
               pointContainer * const pointContainerP,
               vectorContainer * const vectorContainerP,    
               vectorHandling< constValue * > const * const cValP,
               vectorHandling< analyticFunction * > const * const sFunP,
               vectorHandling< analyticGeometry * > const * const depAGeoP );
    virtual vectorHandling< analyticGeometry * > apply( vectorHandling< analyticGeometry * > const * const aGeoVecP ) const;
  private:
    float _parameterOneStartPercent;
    float _parameterTwoStartPercent;
    float _parameterOneEndPercent;
    float _parameterTwoEndPercent;
    vec2dOneD const * _parameterPercentFunction;
    float _maxStep;
    float _tolerance;
    bool _createAnalyticSurface;
    bool _createSplineCurve3d;
  };
}

#endif	/* PICKRANGE_H */

