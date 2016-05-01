#ifndef bSplineCurve2d_angleRatioDeltaYPointOnePointTwoConstructOCC_H
#define	bSplineCurve2d_angleRatioDeltaYPointOnePointTwoConstructOCC_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/ptrHandling.h>

namespace dtOO {
  class dtCurve2d;
  
  class bSplineCurve2d_angleRatioDeltaYPointOnePointTwoConstructOCC {
  public:
    dt__classOnlyName(
      bSplineCurve2d_angleRatioDeltaYPointOnePointTwoConstructOCC
    );
    bSplineCurve2d_angleRatioDeltaYPointOnePointTwoConstructOCC( 
      float angleIn, float angleOut, float const & ratio, float const & deltaY, 
      float const & pointOne, float const & pointTwo
    );
    virtual ~bSplineCurve2d_angleRatioDeltaYPointOnePointTwoConstructOCC();
    dtCurve2d * result( void );
  private:
    ptrHandling<dtCurve2d> _dtC2d;
  };
}
#endif	/* bSplineCurve2d_angleRatioDeltaYPointOnePointTwoConstructOCC_H */

