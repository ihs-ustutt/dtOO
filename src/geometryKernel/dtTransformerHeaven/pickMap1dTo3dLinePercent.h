#ifndef pickMap1dTo3dLinePercent_H
#define	pickMap1dTo3dLinePercent_H

#include "dtTransformer.h"
#include <logMe/dtMacros.h>

namespace dtOO {
  class analyticFunction;
  class vec2dOneD;
  
  class pickMap1dTo3dLinePercent : public dtTransformer {
  public:
    dt__classOnlyName(pickMap1dTo3dLinePercent);   
    pickMap1dTo3dLinePercent();
    pickMap1dTo3dLinePercent(const pickMap1dTo3dLinePercent& orig);
    virtual ~pickMap1dTo3dLinePercent();
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
    float _u0;
    float _u1;
  };
}

#endif	/* pickMap1dTo3dLinePercent_H */

