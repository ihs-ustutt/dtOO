#ifndef GEOMCURVE_PROJECTONGEOMSURFACEOCC_H
#define	GEOMCURVE_PROJECTONGEOMSURFACEOCC_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/ptrHandling.h>
#include <interfaceHeaven/vectorHandling.h>

namespace dtOO {
  class dtCurve;
  class dtSurface;
  
  class geomCurve_projectOnGeomSurfaceOCC {
  public:
    dt__CLASSNAME(geomCurve_projectOnGeomSurfaceOCC);    
    geomCurve_projectOnGeomSurfaceOCC();
    geomCurve_projectOnGeomSurfaceOCC(
      dtCurve const * const dtC, 
      dtSurface const * const dtS
    );
    virtual ~geomCurve_projectOnGeomSurfaceOCC();
    dtCurve * result( void );    
  private:
    ptrHandling< dtCurve > _dtC;
  };
}
#endif	/* GEOMCURVE_PROJECTONGEOMSURFACEOCC_H */

