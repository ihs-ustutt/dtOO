#ifndef PICKMAP3DTO3DRANGEPERCENT_H
#define	PICKMAP3DTO3DRANGEPERCENT_H

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
      QDomElement const * tE, 
      baseContainer const * const bC,
      vectorHandling< constValue * > const * const cV,
      vectorHandling< analyticFunction * > const * const aF,
      vectorHandling< analyticGeometry * > const * const aG 
    );
    virtual vectorHandling< analyticGeometry * > 
    apply( vectorHandling< analyticGeometry * > const * const aGeoVecP ) const;
  private:
    float _u0;
    float _v0;
    float _w0;
  };
}

#endif	/* PICKMAP3DTO3DRANGEPERCENT_H */

