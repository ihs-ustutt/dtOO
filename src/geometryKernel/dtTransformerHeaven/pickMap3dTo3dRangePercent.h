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
      virtual ~pickMap3dTo3dRangePercent();
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
      dtReal _u0;
      dtReal _v0;
      dtReal _w0;
      static bool _registrated;    
  };
}

#endif	/* PICKMAP3DTO3DRANGEPERCENT_H */

