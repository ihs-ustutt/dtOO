#ifndef PICKMAP2DTO3DRANGEPERCENT_H
#define	PICKMAP2DTO3DRANGEPERCENT_H

#include "dtTransformer.h"
#include <logMe/dtMacros.h>

namespace dtOO {
  class analyticFunction;
  class vec2dOneD;
  
  class pickMap2dTo3dRangePercent : public dtTransformer {
  public:
    dt__CLASSNAME(pickMap2dTo3dRangePercent);   
    pickMap2dTo3dRangePercent();
    pickMap2dTo3dRangePercent(const pickMap2dTo3dRangePercent& orig);
    virtual ~pickMap2dTo3dRangePercent();
    virtual dtTransformer * clone( void ) const;
    virtual dtTransformer * create( void ) const;     
    virtual bool isNecessary( void ) const; 
    void init( 
      QDomElement const * tE,
      baseContainer const * const bC,
      vectorHandling< constValue * > const * const cValP,
      vectorHandling< analyticFunction * > const * const sFunP,
      vectorHandling< analyticGeometry * > const * const depAGeoP 
    );
    virtual vectorHandling< analyticGeometry * > 
    apply( vectorHandling< analyticGeometry * > const * const aGeoVecP ) const;
  private:
    float _u0;
    float _v0;
  };
}

#endif	/* PICKMAP2DTO3DRANGEPERCENT_H */

