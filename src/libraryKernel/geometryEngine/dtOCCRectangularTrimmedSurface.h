#ifndef DTOCCRECTANGULARTRIMMEDSURFACE_H
#define	DTOCCRECTANGULARTRIMMEDSURFACE_H

#include <dtOOTypeDef.h>

#include <logMe/dtMacros.h>
#include "dtOCCSurface.h"

class Geom_RectangularTrimmedSurface;

namespace dtOO {
  class dtOCCRectangularTrimmedSurface : public dtOCCSurface {
  public:
    dt__class(dtOCCRectangularTrimmedSurface, dtSurface);
    dtOCCRectangularTrimmedSurface();
    dtOCCRectangularTrimmedSurface(const dtOCCSurfaceBase& orig);
    virtual ~dtOCCRectangularTrimmedSurface();
    virtual dtOCCRectangularTrimmedSurface * clone( void ) const;
	  virtual dtCurve * segmentConstU( dtReal const uu, dtReal const vvMin, dtReal const vvMax) const;
	  virtual dtCurve * segmentConstV( dtReal const vv, dtReal const uuMin, dtReal const uuMax) const;     
  private:
    Geom_RectangularTrimmedSurface const * _ptr;
  };
}
#endif	/* DTOCCRECTANGULARTRIMMEDSURFACE_H */

