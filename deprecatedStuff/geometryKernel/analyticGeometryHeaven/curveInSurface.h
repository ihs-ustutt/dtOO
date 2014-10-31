#ifndef CURVEINSURFACE_H
#define	CURVEINSURFACE_H

#include <dtLinearAlgebra.h>
#include "map1dTo3d.h"
#include <logMe/dtMacros.h>
#include <interfaceHeaven/ptrHandling.h>

namespace dtOO {
  class dtCurve2d;
  class map2dTo3d;
  
  class curveInSurface : public map1dTo3d {
    public:
      dt__CLASSSTD(curveInSurface, analyticGeometry);
      curveInSurface();
      curveInSurface(const curveInSurface& orig);
      curveInSurface(dtCurve2d const * const dtC2d, map2dTo3d const * const m2d);
      curveInSurface(dtCurve2d const * const dtC2d, map2dTo3d const * const m2d, bool const & percentF);
      virtual ~curveInSurface();
      virtual curveInSurface * clone( void ) const;
      virtual curveInSurface * create( void ) const;    
      virtual dtPoint3 getPoint( float const & uu ) const;
      virtual bool isClosed( int const & dir) const;
      virtual float getMin( int const & dir) const;
      virtual float getMax( int const & dir) const;     
  private:
    ptrHandling< dtCurve2d > _dtC2d;
    ptrHandling< map2dTo3d > _m2d;
    bool _percentF;
  };
}
#endif	/* CURVEINSURFACE_H */

