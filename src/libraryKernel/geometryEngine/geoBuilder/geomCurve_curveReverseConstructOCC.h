#ifndef geomCurve_curveReverseConstructOCC_H
#define	geomCurve_curveReverseConstructOCC_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/ptrHandling.h>
#include <interfaceHeaven/vectorHandling.h>

namespace dtOO {
  class dtCurve;
  
  class geomCurve_curveReverseConstructOCC {
  public:
    dt__classOnlyName(geomCurve_curveReverseConstructOCC);    
    geomCurve_curveReverseConstructOCC();
    geomCurve_curveReverseConstructOCC(
      dtCurve const * const dtC, bool const & revU
    );
    virtual ~geomCurve_curveReverseConstructOCC();
    dtCurve * result( void );    
  private:
    ptrHandling< dtCurve > _dtC;
  };
}
#endif	/* geomCurve_curveReverseConstructOCC_H */

