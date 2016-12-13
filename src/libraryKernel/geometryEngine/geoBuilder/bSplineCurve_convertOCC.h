#ifndef bSplineCurve_convertOCC_H
#define	bSplineCurve_convertOCC_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/ptrHandling.h>
#include <interfaceHeaven/vectorHandling.h>

namespace dtOO {
  class dtCurve;
  class dtOCCCurveBase;
  
  class bSplineCurve_convertOCC {
  public:
    dt__classOnlyName(bSplineCurve_convertOCC);
    bSplineCurve_convertOCC( dtCurve const & base );
    virtual ~bSplineCurve_convertOCC();
    dtCurve * result( void );
  private:
    ptrHandling<dtCurve> _dtC;
  };
}
#endif	/* bSplineCurve_convertOCC_H */

