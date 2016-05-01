#ifndef DTOCCRECTANGULARTRIMMEDSURFACE_H
#define	DTOCCRECTANGULARTRIMMEDSURFACE_H

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
    virtual dtSurface * clone( void ) const;
	  virtual dtCurve * segmentConstU( float const uu, float const vvMin, float const vvMax) const;
	  virtual dtCurve * segmentConstV( float const vv, float const uuMin, float const uuMax) const;     
  private:
    Geom_RectangularTrimmedSurface const * _ptr;
  };
}
#endif	/* DTOCCRECTANGULARTRIMMEDSURFACE_H */

