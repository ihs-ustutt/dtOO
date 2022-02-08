#ifndef geomCurve_curveTranslateConstructOCC_H
#define	geomCurve_curveTranslateConstructOCC_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/ptrHandling.h>
#include <interfaceHeaven/vectorHandling.h>

namespace dtOO {
  class dtCurve;
  
  class geomCurve_curveTranslateConstructOCC {
  public:
    dt__classOnlyName(geomCurve_curveTranslateConstructOCC);    
    geomCurve_curveTranslateConstructOCC();
    geomCurve_curveTranslateConstructOCC(
      dtCurve const * const dtC, dtVector3 const & vv
    );
    virtual ~geomCurve_curveTranslateConstructOCC();
    dtCurve * result( void );    
  private:
    ptrHandling< dtCurve > _dtC;
  };
}
#endif	/* geomCurve_curveTranslateConstructOCC_H */

