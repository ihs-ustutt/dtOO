#ifndef PICKVEC3DTWODRANGEPERCENT_H
#define	PICKVEC3DTWODRANGEPERCENT_H

#include "dtTransformer.h"
#include <logMe/dtMacros.h>

namespace dtOO {
  class analyticFunction;
  
  class pickVec3dTwoDRangePercent : public dtTransformer {
  public:
    dt__CLASSNAME(pickVec3dTwoDRangePercent);   
    pickVec3dTwoDRangePercent();
    pickVec3dTwoDRangePercent(const pickVec3dTwoDRangePercent& orig);
    virtual ~pickVec3dTwoDRangePercent();
    virtual dtTransformer * clone( void ) const;
    virtual dtTransformer * create( void ) const;     
    virtual bool isNecessary( void ) const; 
    void init( 
      QDomElement const * tE,
      baseContainer const * const bC,
      vectorHandling< constValue * > const * const cValP,
      vectorHandling< analyticFunction * > const * const sFunP
    );
    virtual vectorHandling< analyticFunction * > 
    apply( vectorHandling< analyticFunction * > const * const aFVecP ) const;
  private:
    float _x0;
    float _x1;
  };
}

#endif	/* PICKVEC3DTWODRANGEPERCENT_H */

