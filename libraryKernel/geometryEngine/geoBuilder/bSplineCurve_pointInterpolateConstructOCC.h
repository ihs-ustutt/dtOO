#ifndef BSPLINECURVE_POINTINTERPOLATECONSTRUCTOCC_H
#define	BSPLINECURVE_POINTINTERPOLATECONSTRUCTOCC_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/ptrHandling.h>

namespace dtOO {
  class dtCurve;
  
  class bSplineCurve_pointInterpolateConstructOCC {
  public:
    dt__CLASSNAME(bSplineCurve_pointInterpolateConstructOCC);
    bSplineCurve_pointInterpolateConstructOCC( std::vector < dtPoint3 > const & pp );
    virtual ~bSplineCurve_pointInterpolateConstructOCC();
    dtCurve * result( void );
  private:
    ptrHandling<dtCurve> _dtC;
  };
}
#endif	/* BSPLINECURVE_POINTINTERPOLATECONSTRUCTOCC_H */

