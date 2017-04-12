#ifndef geomSurface_surfaceTranslateConstructOCC_H
#define	geomSurface_surfaceTranslateConstructOCC_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/ptrHandling.h>
#include <interfaceHeaven/vectorHandling.h>

namespace dtOO {
  class dtSurface;
  
  class geomSurface_surfaceTranslateConstructOCC {
  public:
    dt__classOnlyName(geomSurface_surfaceTranslateConstructOCC);    
    geomSurface_surfaceTranslateConstructOCC();
    geomSurface_surfaceTranslateConstructOCC(
      dtSurface const * const dtS, dtVector3 const & vv
    );
    virtual ~geomSurface_surfaceTranslateConstructOCC();
    dtSurface * result( void );    
  private:
    ptrHandling< dtSurface > _dtS;
  };
}
#endif	/* geomSurface_surfaceTranslateConstructOCC_H */

