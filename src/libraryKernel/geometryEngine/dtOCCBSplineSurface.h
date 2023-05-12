#ifndef DTOCCBSPLINESURFACE_H
#define	DTOCCBSPLINESURFACE_H

#include <dtOOTypeDef.h>

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
    virtual dtOCCBSplineSurface * clone( void ) const;
	  virtual dtCurve * segmentConstU( dtReal const uu, dtReal const vvMin, dtReal const vvMax) const;
	  virtual dtCurve * segmentConstV( dtReal const vv, dtReal const uuMin, dtReal const uuMax) const;     
    virtual dtPoint3 controlPoint( dtInt const uI, dtInt const vI ) const;
    virtual void setControlPoint( dtInt const uI, dtInt const vI, dtPoint3 const point );    
    virtual dtInt nControlPoints( dtInt const dim ) const;   
    virtual std::string dumpToString(void) const;
  private:
    Geom_BSplineSurface const * _ptr;
  };
}
#endif	/* DTOCCBSPLINESURFACE_H */

