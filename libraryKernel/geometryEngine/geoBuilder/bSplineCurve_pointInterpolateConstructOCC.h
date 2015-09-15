#ifndef BSPLINECURVE_POINTINTERPOLATECONSTRUCTOCC_H
#define	BSPLINECURVE_POINTINTERPOLATECONSTRUCTOCC_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>

namespace dtOO {
  class dtCurve;
  
  class bSplineCurve_pointInterpolateConstructOCC {
  public:
    dt__classOnlyName(bSplineCurve_pointInterpolateConstructOCC);
    bSplineCurve_pointInterpolateConstructOCC( 
      std::vector < dtPoint3 > const & pp 
    );
    virtual ~bSplineCurve_pointInterpolateConstructOCC();
    dtCurve * result( void );
  private:
    dt__pH(dtCurve) _dtC;
  };
}
#endif	/* BSPLINECURVE_POINTINTERPOLATECONSTRUCTOCC_H */

