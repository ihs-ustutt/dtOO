#ifndef BSPLINECURVE2D_ANGLERATIODELTAYCONSTRUCTOCC_H
#define	BSPLINECURVE2D_ANGLERATIODELTAYCONSTRUCTOCC_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/ptrHandling.h>

namespace dtOO {
  class dtCurve2d;
  
  class bSplineCurve2d_angleRatioDeltaYConstructOCC {
  public:
    dt__CLASSNAME(bSplineCurve2d_angleRatioDeltaYConstructOCC);
    bSplineCurve2d_angleRatioDeltaYConstructOCC( 
      float angleIn, float angleOut, float const & ratio, float const & deltaY
    );
    virtual ~bSplineCurve2d_angleRatioDeltaYConstructOCC();
    dtCurve2d * result( void );
  private:
    ptrHandling<dtCurve2d> _dtC2d;
  };
}
#endif	/* BSPLINECURVE2D_ANGLERATIODELTAYCONSTRUCTOCC_H */

