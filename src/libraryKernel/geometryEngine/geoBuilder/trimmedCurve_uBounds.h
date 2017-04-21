#ifndef TRIMMEDCURVE_UBOUNDS_H
#define	TRIMMEDCURVE_UBOUNDS_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/ptrHandling.h>

namespace dtOO {
  class dtCurve;
  
  class trimmedCurve_uBounds {
  public:
    dt__classOnlyName(trimmedCurve_uBounds);
    trimmedCurve_uBounds(dtCurve const * dtC, float const & min, float const & max);
    virtual ~trimmedCurve_uBounds();
    dtCurve * result( void );
    private:
      ptrHandling< dtCurve > _dtC;
  };
}
#endif	/* TRIMMEDCURVE_UBOUNDS_H */

