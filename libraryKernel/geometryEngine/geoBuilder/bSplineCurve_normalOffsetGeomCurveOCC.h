#ifndef bSplineCurve_normalOffsetGeomCurveOCC_H
#define	bSplineCurve_normalOffsetGeomCurveOCC_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/ptrHandling.h>

namespace dtOO {
  class dtCurve;
  
  class bSplineCurve_normalOffsetGeomCurveOCC {
  public:
    dt__CLASSNAME(bSplineCurve_normalOffsetGeomCurveOCC);    
    bSplineCurve_normalOffsetGeomCurveOCC(dtCurve const * const dtC, float const & offset);
    virtual ~bSplineCurve_normalOffsetGeomCurveOCC();
    dtCurve * result( void );
  private:
    ptrHandling<dtCurve> _dtC;    
  };
}

#endif	/* bSplineCurve_normalOffsetGeomCurveOCC_H */

