#ifndef PICKLENGTHRANGE_H
#define	PICKLENGTHRANGE_H

#include "dtTransformer.h"
#include <logMe/dtMacros.h>

namespace dtOO {
  class analyticFunction;
  class vec2dOneD;
  
  class pickLengthRange : public dtTransformer{
  public:
    dt__classOnlyName(pickLengthRange);   
    pickLengthRange();
    pickLengthRange(const pickLengthRange& orig);
    virtual ~pickLengthRange();
    virtual dtTransformer * clone( void ) const;
    virtual dtTransformer * create( void ) const;     
    virtual bool isNecessary( void ) const; 
    void init( 
      ::QDomElement const * tE, 
      baseContainer const * const bC,
      vectorHandling< constValue * > const * const cV,
      vectorHandling< analyticFunction * > const * const aF,
      vectorHandling< analyticGeometry * > const * const aG 
    );
    virtual vectorHandling< analyticGeometry * > apply( vectorHandling< analyticGeometry * > const * const aGeoVecP ) const;
  private:
    float _lengthOneStart;
    float _lengthTwoStart;
    float _lengthOneEnd;
    float _lengthTwoEnd;
    bool _createAnalyticSurface;
    bool _createSplineCurve3d;
  };
}

#endif	/* PICKLENGTHRANGE_H */
