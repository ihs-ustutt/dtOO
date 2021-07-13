#ifndef pickVec3dTwoDRectanglePercent_H
#define	pickVec3dTwoDRectanglePercent_H

#include "dtTransformer.h"
#include <logMe/dtMacros.h>

namespace dtOO {
  class analyticFunction;
  
  class pickVec3dTwoDRectanglePercent : public dtTransformer {
  public:
    dt__classOnlyName(pickVec3dTwoDRectanglePercent);   
    pickVec3dTwoDRectanglePercent();
    pickVec3dTwoDRectanglePercent(const pickVec3dTwoDRectanglePercent& orig);
    virtual ~pickVec3dTwoDRectanglePercent();
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
    virtual aFPtrVec apply( aFPtrVec const * const aFVecP ) const;
  private:
    dtPoint2 _p0;
    dtPoint2 _p1;
  };
}

#endif	/* pickVec3dTwoDRectanglePercent_H */