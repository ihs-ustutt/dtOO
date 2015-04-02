#ifndef BSPLINECURVE2D_ANGLEDELTAXDELTAYCONSTRUCTOCC_H
#define	BSPLINECURVE2D_ANGLEDELTAXDELTAYCONSTRUCTOCC_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/ptrHandling.h>

namespace dtOO {
  class dtCurve2d;
  
  class bSplineCurve2d_angleDeltaXDeltaYConstructOCC {
  public:
    dt__classOnlyName(bSplineCurve2d_angleDeltaXDeltaYConstructOCC);
    bSplineCurve2d_angleDeltaXDeltaYConstructOCC( 
      float angleIn, float angleOut, float const & deltaX, float const & deltaY
    );
    virtual ~bSplineCurve2d_angleDeltaXDeltaYConstructOCC();
    dtCurve2d * result( void );
  private:
    ptrHandling<dtCurve2d> _dtC2d;
  };
}
#endif	/* BSPLINECURVE2D_ANGLEDELTAXDELTAYCONSTRUCTOCC_H */

