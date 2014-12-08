#ifndef GEOMCURVE2D_REPARAMINGEOMSURFACEOCC_H
#define	GEOMCURVE2D_REPARAMINGEOMSURFACEOCC_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/ptrHandling.h>
#include <interfaceHeaven/vectorHandling.h>

namespace dtOO {
  class dtCurve;
  class dtCurve2d;
  class dtSurface;
  
  class geomCurve2d_reparamInGeomSurfaceOCC {
  public:
    dt__CLASSNAME(geomCurve2d_reparamInGeomSurfaceOCC);    
    geomCurve2d_reparamInGeomSurfaceOCC();
    geomCurve2d_reparamInGeomSurfaceOCC(
      dtCurve const * const dtC, 
      dtSurface const * const dtS
    );
    virtual ~geomCurve2d_reparamInGeomSurfaceOCC();
    dtCurve2d * result( void );    
  private:
    dt__pH(dtCurve2d) _dtC2d;
  };
}
#endif	/* GEOMCURVE2D_REPARAMINGEOMSURFACEOCC_H */

