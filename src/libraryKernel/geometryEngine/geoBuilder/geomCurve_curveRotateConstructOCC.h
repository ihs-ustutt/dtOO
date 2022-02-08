#ifndef geomCurve_curveRotateConstructOCC_H
#define	geomCurve_curveRotateConstructOCC_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/ptrHandling.h>
#include <interfaceHeaven/vectorHandling.h>

namespace dtOO {
  class dtCurve;
  
  class geomCurve_curveRotateConstructOCC {
  public:
    dt__classOnlyName(geomCurve_curveRotateConstructOCC);    
    geomCurve_curveRotateConstructOCC();
    geomCurve_curveRotateConstructOCC(
      dtCurve const * const dtC, 
      dtPoint3 const & oP, 
      dtVector3 const & rA, 
      dtReal const & angle
    );
    virtual ~geomCurve_curveRotateConstructOCC();
    dtCurve * result( void );    
  private:
    ptrHandling< dtCurve > _dtC;
  };
}
#endif	/* geomCurve_curveRotateConstructOCC_H */

