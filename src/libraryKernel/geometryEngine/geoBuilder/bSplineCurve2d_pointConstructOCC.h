#ifndef BSPLINECURVE2D_POINTCONSTRUCTOCC_H
#define	BSPLINECURVE2D_POINTCONSTRUCTOCC_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/ptrHandling.h>

namespace dtOO {
  class dtCurve2d;
  
  class bSplineCurve2d_pointConstructOCC {
    public:
      dt__classOnlyName(bSplineCurve2d_pointConstructOCC);
      bSplineCurve2d_pointConstructOCC( 
        std::vector< dtPoint2 > const & pp, int const & order 
      );
      bSplineCurve2d_pointConstructOCC( 
        dtPoint2 const & p0,  dtPoint2 const & p1
      );      
      virtual ~bSplineCurve2d_pointConstructOCC();
      dtCurve2d * result( void );
    private:
      ptrHandling<dtCurve2d> _dtC2d;
  };
}
#endif	/* BSPLINECURVE2D_POINTCONSTRUCTOCC_H */

