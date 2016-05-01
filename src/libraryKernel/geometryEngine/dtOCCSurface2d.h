#ifndef DTOCCSURFACE2D_H
#define	DTOCCSURFACE2D_H

#include <interfaceHeaven/ptrHandling.h>
#include "dtOCCSurfaceBase.h"
#include "dtSurface2d.h"

class Geom_Surface;

namespace dtOO {
  class dtOCCSurfaceBase;
  
  class dtOCCSurface2d : public dtSurface2d {
  public:
    dt__class(dtOCCSurface2d, dtSurface2d);    
    dtOCCSurface2d();
    dtOCCSurface2d(const dtOCCSurfaceBase& orig);
    virtual ~dtOCCSurface2d();
    //
    // overload
    //
    virtual float minPara( int const dim ) const;
    virtual float maxPara( int const dim ) const;
    virtual bool closed( int const dim ) const;
    virtual dtPoint2 point( float const uu, float const vv) const;
    virtual std::vector<dtVector2> firstDer( float const uu, float const vv) const;
    virtual std::vector<dtVector2> secondDer( float const uu, float const vv) const;
    virtual dtPoint2 reparam(dtPoint2 const point) const;
    //
    // optional
    //
	  virtual std::string dumpToString( void ) const;
    dtOCCSurfaceBase const & OCCRef( void ) const;
    dtOCCSurfaceBase & OCCRef( void );    
  private:
    ptrHandling< dtOCCSurfaceBase > _surface;
    Geom_Surface const * _ptr;
  };
}
#endif	/* DTOCCSURFACE2D_H */

