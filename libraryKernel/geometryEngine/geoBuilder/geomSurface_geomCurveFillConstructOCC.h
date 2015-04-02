#ifndef GEOMSURFACE_GEOMCURVEFILLCONSTRUCTOCC_H
#define	GEOMSURFACE_GEOMCURVEFILLCONSTRUCTOCC_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/vectorHandling.h>

namespace dtOO {
  class dtCurve;
  class dtSurface;
  
  class geomSurface_geomCurveFillConstructOCC {
    public:
      dt__classOnlyName(geomSurface_geomCurveFillConstructOCC);
      geomSurface_geomCurveFillConstructOCC( vectorHandling< dtCurve const * > const & cc );
      geomSurface_geomCurveFillConstructOCC( dt__pVH(dtCurve) const & cc );      
      geomSurface_geomCurveFillConstructOCC( 
        dtCurve const * c0, dtCurve const * c1, 
        dtCurve const * c2, dtCurve const * c3 
      );
      void init( vectorHandling< dtCurve const * > const & cc );
      virtual ~geomSurface_geomCurveFillConstructOCC();
      dtSurface * result( void );
    private:
      ptrHandling<dtSurface> _dtS;
  };
}
#endif	/* GEOMSURFACE_GEOMCURVEFILLCONSTRUCTOCC_H */

