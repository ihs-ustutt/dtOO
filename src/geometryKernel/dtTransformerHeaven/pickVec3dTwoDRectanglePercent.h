#ifndef pickVec3dTwoDRectanglePercent_H
#define	pickVec3dTwoDRectanglePercent_H

#include <dtOOTypeDef.h>

#include "dtTransformer.h"
#include <logMe/dtMacros.h>

namespace dtOO {
  class analyticFunction;
  
  class pickVec3dTwoDRectanglePercent : public dtTransformer {
    public:
      dt__classOnlyName(pickVec3dTwoDRectanglePercent);   
      pickVec3dTwoDRectanglePercent();
      pickVec3dTwoDRectanglePercent(const pickVec3dTwoDRectanglePercent& orig);
      virtual ~pickVec3dTwoDRectanglePercent();
      virtual dtTransformer * clone( void ) const;
      virtual dtTransformer * create( void ) const;     
      virtual bool isNecessary( void ) const; 
      void init( 
        ::QDomElement const * tE, 
        baseContainer * const bC,
        lvH_constValue const * const cV,
        lvH_analyticFunction const * const aF,
        lvH_analyticGeometry const * const aG 
      );
      virtual lvH_analyticFunction apply( lvH_analyticFunction const * const aFVecP ) const;
    private:
      dtPoint2 _p0;
      dtPoint2 _p1;
      static bool _registrated;    
  };
}

#endif	/* pickVec3dTwoDRectanglePercent_H */