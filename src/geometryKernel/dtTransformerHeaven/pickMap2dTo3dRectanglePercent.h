#ifndef pickMap2dTo3dRectanglePercent_H
#define	pickMap2dTo3dRectanglePercent_H

#include <dtOOTypeDef.h>

#include "dtTransformer.h"
#include <logMe/dtMacros.h>

namespace dtOO {
  class analyticFunction;
//  class vec2dOneD;
  
  class pickMap2dTo3dRectanglePercent : public dtTransformer {
    public:
      dt__classOnlyName(pickMap2dTo3dRectanglePercent);   
      pickMap2dTo3dRectanglePercent();
      pickMap2dTo3dRectanglePercent(const pickMap2dTo3dRectanglePercent& orig);
      pickMap2dTo3dRectanglePercent( jsonPrimitive const & jE );      
      virtual ~pickMap2dTo3dRectanglePercent();
      virtual dtTransformer * clone( void ) const;
      virtual dtTransformer * create( void ) const;     
      virtual bool isNecessary( void ) const;
      virtual void jInit( 
        jsonPrimitive const & jE, 
        baseContainer * const bC,
        lvH_constValue const * const cV,
        lvH_analyticFunction const * const aF,
        lvH_analyticGeometry const * const aG
      );       
      void init( 
        ::QDomElement const * tE, 
        baseContainer * const bC,
        lvH_constValue const * const cV,
        lvH_analyticFunction const * const aF,
        lvH_analyticGeometry const * const aG 
      );
      virtual lvH_analyticGeometry 
      apply( lvH_analyticGeometry const * const aGeoVecP ) const;
    private:
      static bool _registrated;    
  };
}

#endif	/* pickMap2dTo3dRectanglePercent_H */