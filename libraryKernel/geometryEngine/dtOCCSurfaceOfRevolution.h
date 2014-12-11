#ifndef DTOCCSURFACEOFREVOLUTION_H
#define	DTOCCSURFACEOFREVOLUTION_H

#include <logMe/dtMacros.h>
#include "dtOCCSurface.h"

class Geom_SurfaceOfRevolution;

namespace dtOO {
  class dtOCCSurfaceOfRevolution : public dtOCCSurface {
  public:
    dt__CLASSSTD(dtOCCSurfaceOfRevolution, dtSurface);
    dtOCCSurfaceOfRevolution();
    dtOCCSurfaceOfRevolution(const dtOCCSurfaceBase& orig);
    virtual ~dtOCCSurfaceOfRevolution();
    virtual dtSurface * clone( void ) const;
	  virtual dtCurve * segmentConstU( float const uu, float const vvMin, float const vvMax) const;
	  virtual dtCurve * segmentConstV( float const vv, float const uuMin, float const uuMax) const;     
  private:
    Geom_SurfaceOfRevolution const * _ptr;
  };
}
#endif	/* DTOCCSURFACEOFREVOLUTION_H */

