#ifndef GEOMSURFACE_SURFACEROTATECONSTRUCTOCC_H
#define	GEOMSURFACE_SURFACEROTATECONSTRUCTOCC_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/ptrHandling.h>
#include <interfaceHeaven/vectorHandling.h>

namespace dtOO {
  class dtSurface;
  
  class geomSurface_surfaceRotateConstructOCC {
  public:
    dt__CLASSNAME(geomSurface_surfaceRotateConstructOCC);    
    geomSurface_surfaceRotateConstructOCC();
    geomSurface_surfaceRotateConstructOCC(
      dtSurface const * const dtS, 
      dtPoint3 const & oP, 
      dtVector3 const & rA, 
      float const & angle
    );
    virtual ~geomSurface_surfaceRotateConstructOCC();
    dtSurface * result( void );    
  private:
    ptrHandling< dtSurface > _dtS;
  };
}
#endif	/* GEOMSURFACE_SURFACEROTATECONSTRUCTOCC_H */

