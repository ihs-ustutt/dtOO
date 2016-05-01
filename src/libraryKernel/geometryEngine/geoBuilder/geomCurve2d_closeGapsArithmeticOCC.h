#ifndef geomCurve2d_closeGapsArithmeticOCC_H
#define	geomCurve2d_closeGapsArithmeticOCC_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/ptrHandling.h>
#include <interfaceHeaven/vectorHandling.h>

namespace dtOO {
//  class dtCurve;
  class dtCurve2d;
//  class dtSurface;
  
  class geomCurve2d_closeGapsArithmeticOCC {
  public:
    dt__classOnlyName(geomCurve2d_closeGapsArithmeticOCC);    
    geomCurve2d_closeGapsArithmeticOCC();
    geomCurve2d_closeGapsArithmeticOCC(dt__pVH(dtCurve2d) const & dtC2d);
    virtual ~geomCurve2d_closeGapsArithmeticOCC();
    dt__pVH(dtCurve2d) result( void );    
  private:
    dt__pVH(dtCurve2d) _dtC2d;
  };
}
#endif	/* geomCurve2d_closeGapsArithmeticOCC_H */

