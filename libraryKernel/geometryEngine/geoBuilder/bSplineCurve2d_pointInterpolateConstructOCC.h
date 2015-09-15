#ifndef bSplineCurve2d_pointInterpolateConstructOCC_H
#define	bSplineCurve2d_pointInterpolateConstructOCC_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>

namespace dtOO {
  class dtCurve2d;
  
  class bSplineCurve2d_pointInterpolateConstructOCC {
  public:
    dt__classOnlyName(bSplineCurve2d_pointInterpolateConstructOCC);
    bSplineCurve2d_pointInterpolateConstructOCC( 
      std::vector < dtPoint2 > const & pp 
    );
    virtual ~bSplineCurve2d_pointInterpolateConstructOCC();
    dtCurve2d * result( void );
  private:
    dt__pH(dtCurve2d) _dtC;
  };
}
#endif	/* bSplineCurve2d_pointInterpolateConstructOCC_H */

