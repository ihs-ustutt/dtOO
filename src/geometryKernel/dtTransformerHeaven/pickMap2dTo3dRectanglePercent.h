#ifndef pickMap2dTo3dRectanglePercent_H
#define	pickMap2dTo3dRectanglePercent_H

#include "dtTransformer.h"
#include <logMe/dtMacros.h>

namespace dtOO {
  class analyticFunction;
  class vec2dOneD;
  
  class pickMap2dTo3dRectanglePercent : public dtTransformer {
    public:
      dt__classOnlyName(pickMap2dTo3dRectanglePercent);   
      pickMap2dTo3dRectanglePercent();
      pickMap2dTo3dRectanglePercent(const pickMap2dTo3dRectanglePercent& orig);
      virtual ~pickMap2dTo3dRectanglePercent();
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
      virtual aGPtrVec 
      apply( aGPtrVec const * const aGeoVecP ) const;
    private:
      dtPoint2 _p0;
      dtPoint2 _p1;
      static bool _registrated;    
  };
}

#endif	/* pickMap2dTo3dRectanglePercent_H */