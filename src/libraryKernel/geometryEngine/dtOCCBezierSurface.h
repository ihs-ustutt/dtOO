#ifndef DTOCCBEZIERSURFACE_H
#define	DTOCCBEZIERSURFACE_H

#include <dtOOTypeDef.h>

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
    virtual dtOCCBezierSurface * clone( void ) const;
	  virtual dtCurve * segmentConstU( dtReal const uu, dtReal const vvMin, dtReal const vvMax) const;
	  virtual dtCurve * segmentConstV( dtReal const vv, dtReal const uuMin, dtReal const uuMax) const;     
    virtual dtPoint3 controlPoint( dtInt const uI, dtInt const vI ) const;
    virtual void setControlPoint( dtInt const uI, dtInt const vI, dtPoint3 const point );    
    virtual dtInt nControlPoints( dtInt const dim ) const;   
  private:
    Geom_BezierSurface const * _ptr;
  };
}
#endif	/* DTOCCBEZIERSURFACE_H */

