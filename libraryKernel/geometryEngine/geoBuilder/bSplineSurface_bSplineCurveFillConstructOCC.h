#ifndef BSPLINESURFACE_BSPLINECURVEFILLCONSTRUCTOCC_H
#define	BSPLINESURFACE_BSPLINECURVEFILLCONSTRUCTOCC_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/vectorHandling.h>

namespace dtOO {
  class dtCurve;
  class dtSurface;
  
  class bSplineSurface_bSplineCurveFillConstructOCC {
    public:
      dt__classOnlyName(bSplineSurface_bSplineCurveFillConstructOCC);
      bSplineSurface_bSplineCurveFillConstructOCC( vectorHandling< dtCurve const * > const & cc );
      bSplineSurface_bSplineCurveFillConstructOCC( dt__pVH(dtCurve) const & cc );
      virtual ~bSplineSurface_bSplineCurveFillConstructOCC();
      dtSurface * result( void );
    private:
      dt__pH(dtSurface) _dtS;
  };
}
#endif	/* BSPLINESURFACE_BSPLINECURVEFILLCONSTRUCTOCC_H */

