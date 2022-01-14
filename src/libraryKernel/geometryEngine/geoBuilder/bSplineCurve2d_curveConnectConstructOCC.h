#ifndef BSPLINECURVE2D_CURVECONNECTCONSTRUCTOCC_H
#define	BSPLINECURVE2D_CURVECONNECTCONSTRUCTOCC_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/ptrHandling.h>
#include <interfaceHeaven/vectorHandling.h>

namespace dtOO {
  class dtCurve2d;
  
  class bSplineCurve2d_curveConnectConstructOCC {
    public:
      dt__classOnlyName(bSplineCurve2d_curveConnectConstructOCC);
      bSplineCurve2d_curveConnectConstructOCC( vectorHandling< dtCurve2d const * > const & cc );
      virtual ~bSplineCurve2d_curveConnectConstructOCC();
      dtCurve2d * result( void );
    private:
      ptrHandling< dtCurve2d > _dtC2d;
  };
}
#endif	/* BSPLINECURVE_CURVECONNECTCONSTRUCTOCC_H */

