#ifndef PICKVEC3DTWODRANGEPERCENT_H
#define	PICKVEC3DTWODRANGEPERCENT_H

#include <dtOOTypeDef.h>

#include "dtTransformer.h"
#include <logMe/dtMacros.h>

namespace dtOO {
  class analyticFunction;
  
  class pickVec3dTwoDRangePercent : public dtTransformer {
    public:
      dt__classOnlyName(pickVec3dTwoDRangePercent);   
      pickVec3dTwoDRangePercent();
      pickVec3dTwoDRangePercent(const pickVec3dTwoDRangePercent& orig);
      pickVec3dTwoDRangePercent( jsonPrimitive const & jE );
      virtual ~pickVec3dTwoDRangePercent();
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
      virtual aFPtrVec 
      apply( aFPtrVec const * const aFVecP ) const;
    private:
      static bool _registrated;    
  };
}

#endif	/* PICKVEC3DTWODRANGEPERCENT_H */

