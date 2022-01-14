#ifndef BSPLINESURFACE2D_BSPLINECURVE2DFILLCONSTRUCTOCC_H
#define	BSPLINESURFACE2D_BSPLINECURVE2DFILLCONSTRUCTOCC_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/ptrHandling.h>
#include <interfaceHeaven/vectorHandling.h>

namespace dtOO {
  class dtCurve2d;
  class dtSurface2d;
  
  class bSplineSurface2d_bSplineCurve2dFillConstructOCC {
    public:
      dt__classOnlyName(bSplineSurface2d_bSplineCurve2dFillConstructOCC);
      bSplineSurface2d_bSplineCurve2dFillConstructOCC( vectorHandling< dtCurve2d const * > const & cc2d );
      virtual ~bSplineSurface2d_bSplineCurve2dFillConstructOCC();
      dtSurface2d * result( void );
    private:
      ptrHandling<dtSurface2d> _dtS2d;
  };
}
#endif	/* BSPLINESURFACE2D_BSPLINECURVE2DFILLCONSTRUCTOCC_H */

