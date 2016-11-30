#ifndef CLOSEGAPS_H
#define	CLOSEGAPS_H

#include "dtTransformer.h"
#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>

namespace dtOO {
  class analyticSurface;
  
  class closeGaps : public dtTransformer {
  public:    
    dt__classOnlyName(closeGaps);
    closeGaps();
    virtual ~closeGaps();
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
    virtual aGPtrVec apply( 
      aGPtrVec const * const aGeoVecP 
    ) const;
  private:
    dt__pH(analyticSurface) _vvStartAGeo;
    dt__pH(analyticSurface) _vvEndAGeo;
  };
}
#endif	/* CLOSEGAPS_H */

