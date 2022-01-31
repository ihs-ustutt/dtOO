#ifndef DTOCCSURFACE_H
#define	DTOCCSURFACE_H

#include <dtOOTypeDef.h>

#include <interfaceHeaven/ptrHandling.h>
#include "dtSurface.h"

class Geom_Surface;

namespace dtOO {
  class dtOCCSurfaceBase;
  
  class dtOCCSurface : public dtSurface {
  public:
    dt__class(dtOCCSurface, dtSurface);    
    dtOCCSurface();
    dtOCCSurface(const dtOCCSurfaceBase& orig);
    virtual ~dtOCCSurface();
    //
    // overload
    //
    virtual dtReal minPara( dtInt const dim ) const;
    virtual dtReal maxPara( dtInt const dim ) const;
    virtual bool closed( dtInt const dim ) const;
    virtual dtPoint3 point( dtReal const uu, dtReal const vv) const;
    virtual dtVector3 normal( dtReal const uu, dtReal const vv) const;
    virtual std::vector<dtVector3> firstDer( dtReal const uu, dtReal const vv) const;
    virtual std::vector<dtVector3> secondDer( dtReal const uu, dtReal const vv) const;
    virtual dtPoint2 reparam(dtPoint3 const point) const;
    //
    // optional
    //
	  virtual std::string dumpToString(void) const;
    dtOCCSurfaceBase const & OCCRef(void) const;
    dtOCCSurfaceBase & OCCRef(void);  
  private:
    ptrHandling< dtOCCSurfaceBase > _surface;
    Geom_Surface const * _ptr;
  };
}
#endif	/* DTOCCSURFACE_H */

