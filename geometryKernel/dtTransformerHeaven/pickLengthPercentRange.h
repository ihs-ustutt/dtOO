#ifndef PICKLENGTHPERCENTRANGE_H
#define	PICKLENGTHPERCENTRANGE_H

#include "dtTransformer.h"
#include <logMe/dtMacros.h>

namespace dtOO {
  class analyticFunction;
  class vec2dOneD;
  
  class pickLengthPercentRange : public dtTransformer{
  public:
    dt__CLASSNAME(pickLengthPercentRange);   
    pickLengthPercentRange();
    pickLengthPercentRange(const pickLengthPercentRange& orig);    
    virtual ~pickLengthPercentRange();
    virtual dtTransformer * clone( void ) const;
    virtual dtTransformer * create( void ) const;     
    virtual bool isNecessary( void ) const; 
    void init( 
      QDomElement * transformerElementP, 
      baseContainer * const bC,
      vectorHandling< constValue * > const * const cValP,
      vectorHandling< analyticFunction * > const * const sFunP,
      vectorHandling< analyticGeometry * > const * const depAGeoP 
    );
    virtual vectorHandling< analyticGeometry * > apply( vectorHandling< analyticGeometry * > const * const aGeoVecP ) const;
  private:
    float _lengthPercentOneStart;
    float _lengthPercentTwoStart;
    float _lengthPercentOneEnd;
    float _lengthPercentTwoEnd;
    bool _createAnalyticSurface;
    bool _createSplineCurve3d;
  };
}

#endif	/* PICKLENGTHPERCENTRANGE_H */

