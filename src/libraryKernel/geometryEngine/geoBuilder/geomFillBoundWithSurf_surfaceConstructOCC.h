#ifndef geomFillBoundWithSurf_surfaceConstructOCC_H
#define	geomFillBoundWithSurf_surfaceConstructOCC_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/ptrHandling.h>

namespace dtOO {
  class dtCurve;
  class dtSurface;
  class dtCurve2d;
  
  class geomFillBoundWithSurf_surfaceConstructOCC {
  public:
    dt__classOnlyName(geomFillBoundWithSurf_surfaceConstructOCC);
    geomFillBoundWithSurf_surfaceConstructOCC( 
      dtSurface const * const dtS, dtCurve2d const * const dtC2d
    );
    virtual ~geomFillBoundWithSurf_surfaceConstructOCC();
    dtCurve * result( void );
  private:
    dt__pH(dtCurve) _dtC;
  };
}
#endif	/* geomFillBoundWithSurf_surfaceConstructOCC_H */

