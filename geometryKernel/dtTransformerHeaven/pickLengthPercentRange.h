#ifndef PICKLENGTHPERCENTRANGE_H
#define	PICKLENGTHPERCENTRANGE_H

#include "dtTransformer.h"
#include <logMe/dtMacros.h>

namespace dtOO {
  class analyticFunction;
  class vec2dOneD;
  
  class pickLengthPercentRange : public dtTransformer{
  public:
    dt__classOnlyName(pickLengthPercentRange);   
    pickLengthPercentRange();
    pickLengthPercentRange(const pickLengthPercentRange& orig);    
    virtual ~pickLengthPercentRange();
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

