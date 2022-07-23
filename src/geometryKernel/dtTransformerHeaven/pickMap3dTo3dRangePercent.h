#ifndef PICKMAP3DTO3DRANGEPERCENT_H
#define	PICKMAP3DTO3DRANGEPERCENT_H

#include <dtOOTypeDef.h>
#include "dtTransformer.h"
#include <logMe/dtMacros.h>

namespace dtOO {
  class analyticFunction;
  class vec2dOneD;
  
  class pickMap3dTo3dRangePercent : public dtTransformer {
    public:
      dt__classOnlyName(pickMap3dTo3dRangePercent);   
      pickMap3dTo3dRangePercent();
      pickMap3dTo3dRangePercent(const pickMap3dTo3dRangePercent& orig);
      pickMap3dTo3dRangePercent( jsonPrimitive const & jE );
      virtual ~pickMap3dTo3dRangePercent();
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

#endif	/* PICKMAP3DTO3DRANGEPERCENT_H */

