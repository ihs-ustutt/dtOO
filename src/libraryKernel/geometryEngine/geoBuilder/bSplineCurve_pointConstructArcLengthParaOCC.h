#ifndef BSPLINECURVE_POINTCONSTRUCTARCLENGTHPARAOCC_H
#define	BSPLINECURVE_POINTCONSTRUCTARCLENGTHPARAOCC_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/ptrHandling.h>

namespace dtOO {
  class dtCurve;
  
  class bSplineCurve_pointConstructArcLengthParaOCC {
  public:
    dt__classOnlyName(bSplineCurve_pointConstructArcLengthParaOCC);
    bSplineCurve_pointConstructArcLengthParaOCC( 
      dtCurve const * dtC, dtReal const tol, 
      dtInt const maxOrder, dtInt const segments       
    );
    virtual ~bSplineCurve_pointConstructArcLengthParaOCC();
    dtCurve * result( void );
  private:
    ptrHandling<dtCurve> _dtC;
  };
}
#endif	/* BSPLINECURVE_POINTCONSTRUCTARCLENGTHPARAOCC_H */

