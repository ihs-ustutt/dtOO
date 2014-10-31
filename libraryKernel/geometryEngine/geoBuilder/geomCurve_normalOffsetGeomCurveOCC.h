#ifndef GEOMCURVE_NORMALOFFSETGEOMCURVEOCC_H
#define	GEOMCURVE_NORMALOFFSETGEOMCURVEOCC_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/ptrHandling.h>

namespace dtOO {
  class dtCurve;
  
  class geomCurve_normalOffsetGeomCurveOCC {
  public:
    dt__CLASSNAME(geomCurve_normalOffsetGeomCurveOCC);    
    geomCurve_normalOffsetGeomCurveOCC(dtCurve const * const dtC, float const & offset);
    virtual ~geomCurve_normalOffsetGeomCurveOCC();
    dtCurve * result( void );
  private:
    ptrHandling<dtCurve> _dtC;    
  };
}

#endif	/* GEOMCURVE_NORMALOFFSETGEOMCURVEOCC_H */

