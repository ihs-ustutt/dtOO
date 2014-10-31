#ifndef TRIMMEDCURVE2D_TWOPOINTSCONNECTCONSTRUCT_H
#define	TRIMMEDCURVE2D_TWOPOINTSCONNECTCONSTRUCT_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/ptrHandling.h>

namespace dtOO {
  class dtCurve2d;
  
  class trimmedCurve2d_twoPointsConnectConstructOCC {
  public:
    dt__CLASSNAME(trimmedCurve2d_twoPointsConnectConstructOCC);
    trimmedCurve2d_twoPointsConnectConstructOCC( dtPoint2 const & p0, dtPoint2 const & p1 );
    virtual ~trimmedCurve2d_twoPointsConnectConstructOCC();
    dtCurve2d * result( void );
  private:
    ptrHandling<dtCurve2d> _dtC2d;
  };
}
#endif	/* TRIMMEDCURVE2D_TWOPOINTSCONNECTCONSTRUCT_H */

