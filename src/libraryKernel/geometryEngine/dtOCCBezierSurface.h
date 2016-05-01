#ifndef DTOCCBEZIERSURFACE_H
#define	DTOCCBEZIERSURFACE_H

#include <logMe/dtMacros.h>
#include "dtOCCSurface.h"

class Geom_BezierSurface;

namespace dtOO {
  class dtOCCSurfaceBase;
  
  class dtOCCBezierSurface : public dtOCCSurface {
  public:
    dt__class(dtOCCBezierSurface, dtSurface);
    dtOCCBezierSurface();
    dtOCCBezierSurface(const dtOCCSurfaceBase& orig);
    virtual ~dtOCCBezierSurface();
    virtual dtSurface * clone( void ) const;
	  virtual dtCurve * segmentConstU( float const uu, float const vvMin, float const vvMax) const;
	  virtual dtCurve * segmentConstV( float const vv, float const uuMin, float const uuMax) const;     
    virtual dtPoint3 controlPoint( int const uI, int const vI ) const;
    virtual void setControlPoint( int const uI, int const vI, dtPoint3 const point );    
    virtual int nControlPoints( int const dim ) const;   
  private:
    Geom_BezierSurface const * _ptr;
  };
}
#endif	/* DTOCCBEZIERSURFACE_H */

