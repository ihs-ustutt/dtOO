#ifndef geomCurve_closeGapsArithmeticOCC_H
#define	geomCurve_closeGapsArithmeticOCC_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/ptrHandling.h>
#include <interfaceHeaven/vectorHandling.h>

namespace dtOO {
  class dtCurve;
  
  class geomCurve_closeGapsArithmeticOCC {
  public:
    dt__classOnlyName(geomCurve_closeGapsArithmeticOCC);    
    geomCurve_closeGapsArithmeticOCC();
    geomCurve_closeGapsArithmeticOCC(dt__pVH(dtCurve) const & dtC);
    virtual ~geomCurve_closeGapsArithmeticOCC();
    dt__pVH(dtCurve) result( void );    
  private:
    dt__pVH(dtCurve) _dtC;
  };
}
#endif	/* geomCurve_closeGapsArithmeticOCC_H */

