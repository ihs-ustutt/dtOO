#ifndef DTOCCBSPLINESURFACE_H
#define	DTOCCBSPLINESURFACE_H

#include <logMe/dtMacros.h>
#include "dtOCCSurface.h"

class Geom_BSplineSurface;

namespace dtOO {
  class dtOCCSurfaceBase;
  
  class dtOCCBSplineSurface : public dtOCCSurface {
  public:
    dt__class(dtOCCBSplineSurface, dtSurface);
    dtOCCBSplineSurface();
    dtOCCBSplineSurface(const dtOCCSurfaceBase& orig);
    virtual ~dtOCCBSplineSurface();
    virtual dtSurface * clone( void ) const;
	  virtual dtCurve * segmentConstU( float const uu, float const vvMin, float const vvMax) const;
	  virtual dtCurve * segmentConstV( float const vv, float const uuMin, float const uuMax) const;     
    virtual dtPoint3 controlPoint( int const uI, int const vI ) const;
    virtual void setControlPoint( int const uI, int const vI, dtPoint3 const point );    
    virtual int nControlPoints( int const dim ) const;   
    virtual std::string dumpToString(void) const;
  private:
    Geom_BSplineSurface const * _ptr;
  };
}
#endif	/* DTOCCBSPLINESURFACE_H */

