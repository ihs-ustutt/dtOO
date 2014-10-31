#ifndef RECTANGULARTRIMMEDSURFACE_UVBOUNDS_H
#define	RECTANGULARTRIMMEDSURFACE_UVBOUNDS_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/ptrHandling.h>

namespace dtOO {
  class dtSurface;
  class dtCurve;
  
  class rectangularTrimmedSurface_uvBounds {
  public:
    dt__CLASSNAME(rectangularTrimmedSurface_uvBounds);
    rectangularTrimmedSurface_uvBounds(dtSurface const * dtS, dtPoint2 const & min, dtPoint2 const & max);
    virtual ~rectangularTrimmedSurface_uvBounds();
    dtSurface * result( void );
    private:
      ptrHandling<dtSurface> _dtS;
  };
}
#endif	/* RECTANGULARTRIMMEDSURFACE_UVBOUNDS_H */

