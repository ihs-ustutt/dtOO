#ifndef BSPLINECURVE2D_ANGLERATIODELTAYCONSTRUCTOCC_H
#define	BSPLINECURVE2D_ANGLERATIODELTAYCONSTRUCTOCC_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/ptrHandling.h>

namespace dtOO {
  class dtCurve2d;
  
  class bSplineCurve2d_angleRatioDeltaYConstructOCC {
  public:
    dt__classOnlyName(bSplineCurve2d_angleRatioDeltaYConstructOCC);
    bSplineCurve2d_angleRatioDeltaYConstructOCC( 
      dtReal angleIn, dtReal angleOut, dtReal const & ratio, dtReal const & deltaY
    );
    virtual ~bSplineCurve2d_angleRatioDeltaYConstructOCC();
    dtCurve2d * result( void );
  private:
    ptrHandling<dtCurve2d> _dtC2d;
  };
}
#endif	/* BSPLINECURVE2D_ANGLERATIODELTAYCONSTRUCTOCC_H */

