#ifndef dtOCCCompositeSurface_H
#define	dtOCCCompositeSurface_H

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
      float const uu, float const vvMin, float const vvMax
    ) const;
	  virtual dtCurve * segmentConstV( 
      float const vv, float const uuMin, float const uuMax
    ) const;     
  private:
    ShapeExtend_CompositeSurface const * _ptr;
  };
}
#endif	/* dtOCCCompositeSurface_H */

