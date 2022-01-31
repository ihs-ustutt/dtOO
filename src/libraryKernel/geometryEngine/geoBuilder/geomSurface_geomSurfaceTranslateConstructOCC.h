#ifndef GEOMSURFACE_GEOMSURFACETRANSLATECONSTRUCTOCC_H
#define	GEOMSURFACE_GEOMSURFACETRANSLATECONSTRUCTOCC_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/ptrHandling.h>

namespace dtOO {
  class dtSurface;
  
  class geomSurface_geomSurfaceTranslateConstructOCC {
    public:
      dt__classOnlyName(geomSurface_geomSurfaceTranslateConstructOCC);
      geomSurface_geomSurfaceTranslateConstructOCC( 
        dtSurface const * const ss, dtVector3 const & vv 
      );
      virtual ~geomSurface_geomSurfaceTranslateConstructOCC();
      dtSurface * result( void );
    private:
      ptrHandling<dtSurface> _dtS;
  };
}
#endif	/* GEOMSURFACE_GEOMSURFACETRANSLATECONSTRUCTOCC_H */

