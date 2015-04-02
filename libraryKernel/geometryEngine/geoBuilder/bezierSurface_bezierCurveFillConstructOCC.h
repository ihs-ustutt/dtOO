#ifndef BEZIERSURFACE_BEZIERCURVEFILLCONSTRUCTOCC_H
#define	BEZIERSURFACE_BEZIERCURVEFILLCONSTRUCTOCC_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/ptrHandling.h>
#include <interfaceHeaven/vectorHandling.h>

namespace dtOO {
  class dtCurve;
  class dtSurface;
  
  class bezierSurface_bezierCurveFillConstructOCC {
    public:
      dt__classOnlyName(bezierSurface_bezierCurveFillConstructOCC);
      bezierSurface_bezierCurveFillConstructOCC( vectorHandling< dtCurve const * > const & cc );
      virtual ~bezierSurface_bezierCurveFillConstructOCC();
      dtSurface * result( void );
    private:
      ptrHandling<dtSurface> _dtS;
  };
}
#endif	/* BEZIERSURFACE_BEZIERCURVEFILLCONSTRUCT_H */

