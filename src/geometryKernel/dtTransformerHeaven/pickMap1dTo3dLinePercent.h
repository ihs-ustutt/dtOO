#ifndef pickMap1dTo3dLinePercent_H
#define	pickMap1dTo3dLinePercent_H

#include <dtOOTypeDef.h>

#include "dtTransformer.h"
#include <logMe/dtMacros.h>

namespace dtOO {
  class analyticFunction;
  class vec2dOneD;
  
  //! Trim segment between [_u0, _u1].
  class pickMap1dTo3dLinePercent : public dtTransformer {
    public:
      dt__classOnlyName(pickMap1dTo3dLinePercent);   
      pickMap1dTo3dLinePercent();
      pickMap1dTo3dLinePercent(const pickMap1dTo3dLinePercent& orig);
      pickMap1dTo3dLinePercent( jsonPrimitive const & jE );      
      virtual ~pickMap1dTo3dLinePercent();
      virtual dtTransformer * clone( void ) const;
      virtual dtTransformer * create( void ) const;     
      virtual bool isNecessary( void ) const; 
      virtual void jInit( 
        jsonPrimitive const & jE, 
        baseContainer * const bC,
        cVPtrVec const * const cV,
        aFPtrVec const * const aF,
        aGPtrVec const * const aG
      );
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
      static bool _registrated;    
  };
}

#endif	/* pickMap1dTo3dLinePercent_H */

