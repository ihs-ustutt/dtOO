#ifndef BSPLINECURVE_CURVECONNECTCONSTRUCTOCC_H
#define	BSPLINECURVE_CURVECONNECTCONSTRUCTOCC_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/ptrHandling.h>
#include <interfaceHeaven/vectorHandling.h>

namespace dtOO {
  class dtCurve;
  
  class bSplineCurve_curveConnectConstructOCC {
    public:
      dt__classOnlyName(bSplineCurve_curveConnectConstructOCC);
      bSplineCurve_curveConnectConstructOCC( 
        dtCurve const * const c0,  dtCurve const * const c1 
      );    
      bSplineCurve_curveConnectConstructOCC( 
        vectorHandling< dtCurve const * > const & cc,
        float const & tol, int const & param, int const & minM
      );
      bSplineCurve_curveConnectConstructOCC( 
        vectorHandling< dtCurve const * > const & cc
      );    
      virtual ~bSplineCurve_curveConnectConstructOCC();
      dtCurve * result( void );
    private:
      ptrHandling<dtCurve> _dtC;
  };
}
#endif	/* BSPLINECURVE_CURVECONNECTCONSTRUCTOCC_H */

