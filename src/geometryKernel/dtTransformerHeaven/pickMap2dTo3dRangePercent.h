#ifndef PICKMAP2DTO3DRANGEPERCENT_H
#define	PICKMAP2DTO3DRANGEPERCENT_H

#include "dtTransformer.h"
#include <logMe/dtMacros.h>

namespace dtOO {
  class analyticFunction;
  class vec2dOneD;
  
  class pickMap2dTo3dRangePercent : public dtTransformer {
  public:
    dt__classOnlyName(pickMap2dTo3dRangePercent);   
    pickMap2dTo3dRangePercent();
    pickMap2dTo3dRangePercent(const pickMap2dTo3dRangePercent& orig);
    virtual ~pickMap2dTo3dRangePercent();
    virtual dtTransformer * clone( void ) const;
    virtual dtTransformer * create( void ) const;     
    virtual bool isNecessary( void ) const; 
    void init( 
      ::QDomElement const * tE, 
      baseContainer const * const bC,
      cVPtrVec const * const cV,
      aFPtrVec const * const aF,
      aGPtrVec const * const aG 
    );
    virtual aGPtrVec 
    apply( aGPtrVec const * const aGeoVecP ) const;
  private:
    float _u0;
    float _v0;
  };
}

#endif	/* PICKMAP2DTO3DRANGEPERCENT_H */

