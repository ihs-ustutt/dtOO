#ifndef BSPLINECURVE_POINTCONSTRUCTOCC_H
#define	BSPLINECURVE_POINTCONSTRUCTOCC_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/ptrHandling.h>

namespace dtOO {
  class dtCurve;
  
  class bSplineCurve_pointConstructOCC {
  public:
    dt__classOnlyName(bSplineCurve_pointConstructOCC);
    bSplineCurve_pointConstructOCC( std::vector< dtPoint3 > const & pp, int const & order );
    virtual ~bSplineCurve_pointConstructOCC();
    dtCurve * result( void );
  private:
    ptrHandling<dtCurve> _dtC;
  };
}
#endif	/* BSPLINECURVE_POINTCONSTRUCTOCC_H */

