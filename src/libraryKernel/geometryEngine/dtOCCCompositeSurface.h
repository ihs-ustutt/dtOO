#ifndef dtOCCCompositeSurface_H
#define	dtOCCCompositeSurface_H

#include <dtOOTypeDef.h>

#include <logMe/dtMacros.h>
#include "dtOCCSurface.h"

class ShapeExtend_CompositeSurface;

namespace dtOO {
  class dtOCCSurfaceBase;
  
  class dtOCCCompositeSurface : public dtOCCSurface {
  public:
    dt__class(dtOCCCompositeSurface, dtSurface);
    dtOCCCompositeSurface();
    dtOCCCompositeSurface(const dtOCCSurfaceBase& orig);
    virtual ~dtOCCCompositeSurface();
    virtual dtSurface * clone( void ) const;
	  virtual dtCurve * segmentConstU( 
      dtReal const uu, dtReal const vvMin, dtReal const vvMax
    ) const;
	  virtual dtCurve * segmentConstV( 
      dtReal const vv, dtReal const uuMin, dtReal const uuMax
    ) const;     
  private:
    ShapeExtend_CompositeSurface const * _ptr;
  };
}
#endif	/* dtOCCCompositeSurface_H */

