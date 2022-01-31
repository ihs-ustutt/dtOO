#ifndef bSplineCurve2d_angleDeltaXDeltaYPointOnePointTwoConstructOCC_H
#define	bSplineCurve2d_angleDeltaXDeltaYPointOnePointTwoConstructOCC_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/ptrHandling.h>

namespace dtOO {
  class dtCurve2d;
  
  class bSplineCurve2d_angleDeltaXDeltaYPointOnePointTwoConstructOCC {
  public:
    dt__classOnlyName(
      bSplineCurve2d_angleDeltaXDeltaYPointOnePointTwoConstructOCC
    );
    bSplineCurve2d_angleDeltaXDeltaYPointOnePointTwoConstructOCC( 
      dtReal angleIn, dtReal angleOut, dtReal const & deltaX, dtReal const & deltaY, 
      dtReal const & pointOne, dtReal const & pointTwo
    );
    virtual ~bSplineCurve2d_angleDeltaXDeltaYPointOnePointTwoConstructOCC();
    dtCurve2d * result( void );
  private:
    ptrHandling<dtCurve2d> _dtC2d;
  };
}
#endif	/* bSplineCurve2d_angleDeltaXDeltaYPointOnePointTwoConstructOCC_H */

