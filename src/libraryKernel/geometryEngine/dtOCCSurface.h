#ifndef DTOCCSURFACE_H
#define	DTOCCSURFACE_H

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
    virtual float minPara( int const dim ) const;
    virtual float maxPara( int const dim ) const;
    virtual bool closed( int const dim ) const;
    virtual dtPoint3 point( float const uu, float const vv) const;
    virtual dtVector3 normal( float const uu, float const vv) const;
    virtual std::vector<dtVector3> firstDer( float const uu, float const vv) const;
    virtual std::vector<dtVector3> secondDer( float const uu, float const vv) const;
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

