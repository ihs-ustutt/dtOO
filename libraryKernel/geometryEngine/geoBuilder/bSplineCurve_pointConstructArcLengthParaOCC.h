#ifndef BSPLINECURVE_POINTCONSTRUCTARCLENGTHPARAOCC_H
#define	BSPLINECURVE_POINTCONSTRUCTARCLENGTHPARAOCC_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/ptrHandling.h>

namespace dtOO {
  class dtCurve;
  
  class bSplineCurve_pointConstructArcLengthParaOCC {
  public:
    dt__CLASSNAME(bSplineCurve_pointConstructArcLengthParaOCC);
    bSplineCurve_pointConstructArcLengthParaOCC( 
      dtCurve const * dtC, float const tol, 
      int const maxOrder, int const segments       
    );
    virtual ~bSplineCurve_pointConstructArcLengthParaOCC();
    dtCurve * result( void );
  private:
    ptrHandling<dtCurve> _dtC;
  };
}
#endif	/* BSPLINECURVE_POINTCONSTRUCTARCLENGTHPARAOCC_H */

