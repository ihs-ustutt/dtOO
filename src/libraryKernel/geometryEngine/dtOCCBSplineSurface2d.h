#ifndef DTOCCBSPLINESURFACE2D_H
#define	DTOCCBSPLINESURFACE2D_H

#include <dtOOTypeDef.h>

#include <logMe/dtMacros.h>
#include "dtOCCSurface2d.h"

class Geom_BSplineSurface;

namespace dtOO {
  class dtOCCSurfaceBase;
  
  class dtOCCBSplineSurface2d : public dtOCCSurface2d {
  public:
    dt__class(dtOCCBSplineSurface2d, dtSurface2d);
    dtOCCBSplineSurface2d();
    dtOCCBSplineSurface2d(const dtOCCSurfaceBase& orig);
    virtual ~dtOCCBSplineSurface2d();
    virtual dtSurface2d * clone( void ) const;
	  virtual dtCurve2d * segmentConstU( dtReal const uu, dtReal const vvMin, dtReal const vvMax) const;
	  virtual dtCurve2d * segmentConstV( dtReal const vv, dtReal const uuMin, dtReal const uuMax) const;     
    virtual dtPoint2 controlPoint( int const uI, int const vI ) const;
    virtual void setControlPoint( int const uI, int const vI, dtPoint2 const point );    
    virtual int nControlPoints( int const dim ) const;   
  private:
    Geom_BSplineSurface const * _ptr;
  };
}
#endif	/* DTOCCBSPLINESURFACE2D_H */

