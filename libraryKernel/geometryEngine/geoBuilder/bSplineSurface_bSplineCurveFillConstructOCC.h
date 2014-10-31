#ifndef BSPLINESURFACE_BSPLINECURVEFILLCONSTRUCTOCC_H
#define	BSPLINESURFACE_BSPLINECURVEFILLCONSTRUCTOCC_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/ptrHandling.h>
#include <interfaceHeaven/vectorHandling.h>

namespace dtOO {
  class dtCurve;
  class dtSurface;
  
  class bSplineSurface_bSplineCurveFillConstructOCC {
    public:
      dt__CLASSNAME(bSplineSurface_bSplineCurveFillConstructOCC);
      bSplineSurface_bSplineCurveFillConstructOCC( vectorHandling< dtCurve const * > const & cc );
      virtual ~bSplineSurface_bSplineCurveFillConstructOCC();
      dtSurface * result( void );
    private:
      ptrHandling<dtSurface> _dtS;
  };
}
#endif	/* BSPLINESURFACE_BSPLINECURVEFILLCONSTRUCTOCC_H */

