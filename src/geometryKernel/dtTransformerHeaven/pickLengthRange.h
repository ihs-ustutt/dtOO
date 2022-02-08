#ifndef PICKLENGTHRANGE_H
#define	PICKLENGTHRANGE_H

#include <dtOOTypeDef.h>

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
        baseContainer * const bC,
        cVPtrVec const * const cV,
        aFPtrVec const * const aF,
        aGPtrVec const * const aG 
      );
      virtual aGPtrVec apply( aGPtrVec const * const aGeoVecP ) const;
    private:
      dtReal _lengthOneStart;
      dtReal _lengthTwoStart;
      dtReal _lengthOneEnd;
      dtReal _lengthTwoEnd;
      bool _createAnalyticSurface;
      bool _createSplineCurve3d;
      static bool _registrated;    
  };
}

#endif	/* PICKLENGTHRANGE_H */

