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
        ::QDomElement const * tE, 
        baseContainer * const bC,
        cVPtrVec const * const cV,
        aFPtrVec const * const aF,
        aGPtrVec const * const aG 
      );
      virtual aGPtrVec apply( aGPtrVec const * const aGeoVecP ) const;
    private:
      float _lengthPercentOneStart;
      float _lengthPercentTwoStart;
      float _lengthPercentOneEnd;
      float _lengthPercentTwoEnd;
      bool _createAnalyticSurface;
      bool _createSplineCurve3d;
      static bool _registrated;    
  };
}

#endif	/* PICKLENGTHPERCENTRANGE_H */

