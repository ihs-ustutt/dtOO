#ifndef DTOCCRECTANGULARTRIMMEDSURFACE_H
#define	DTOCCRECTANGULARTRIMMEDSURFACE_H

#include <logMe/dtMacros.h>
#include "dtOCCSurface.h"

class Geom_RectangularTrimmedSurface;

namespace dtOO {
  class dtOCCRectangularTrimmedSurface : public dtOCCSurface {
  public:
    dt__CLASSSTD(dtOCCRectangularTrimmedSurface, dtSurface);
    dtOCCRectangularTrimmedSurface();
    dtOCCRectangularTrimmedSurface(const dtOCCSurfaceBase& orig);
    virtual ~dtOCCRectangularTrimmedSurface();
    virtual dtSurface * clone( void ) const;
	  virtual dtCurve * getCurveConstU( float const uu, float const vvMin, float const vvMax) const;
	  virtual dtCurve * getCurveConstV( float const vv, float const uuMin, float const uuMax) const;     
    virtual dtPoint3 controlPoint( int const uI, int const vI ) const;
    virtual void setControlPoint( int const uI, int const vI, dtPoint3 const point );    
    virtual int nControlPoints( int const dim ) const;  
  private:
    Geom_RectangularTrimmedSurface const * _ptr;
  };
}
#endif	/* DTOCCRECTANGULARTRIMMEDSURFACE_H */

