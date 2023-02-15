#ifndef DTOCCSURFACEOFREVOLUTION_H
#define	DTOCCSURFACEOFREVOLUTION_H

#include <dtOOTypeDef.h>

#include <logMe/dtMacros.h>
#include "dtOCCSurface.h"

class Geom_SurfaceOfRevolution;

namespace dtOO {
  class dtOCCSurfaceOfRevolution : public dtOCCSurface {
  public:
    dt__class(dtOCCSurfaceOfRevolution, dtSurface);
    dtOCCSurfaceOfRevolution();
    dtOCCSurfaceOfRevolution(const dtOCCSurfaceBase& orig);
    virtual ~dtOCCSurfaceOfRevolution();
    virtual dtOCCSurfaceOfRevolution * clone( void ) const;
	  virtual dtCurve * segmentConstU( dtReal const uu, dtReal const vvMin, dtReal const vvMax) const;
	  virtual dtCurve * segmentConstV( dtReal const vv, dtReal const uuMin, dtReal const uuMax) const;     
  private:
    Geom_SurfaceOfRevolution const * _ptr;
  };
}
#endif	/* DTOCCSURFACEOFREVOLUTION_H */

