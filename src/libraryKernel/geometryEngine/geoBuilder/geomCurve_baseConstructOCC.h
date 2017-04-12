#ifndef geomCurve_baseConstructOCC_H
#define	geomCurve_baseConstructOCC_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/ptrHandling.h>
#include <interfaceHeaven/vectorHandling.h>

namespace dtOO {
  class dtCurve;
  class dtOCCCurveBase;
  
  class geomCurve_baseConstructOCC {
  public:
    dt__classOnlyName(geomCurve_baseConstructOCC);
    geomCurve_baseConstructOCC( dtOCCCurveBase const & base );
    virtual ~geomCurve_baseConstructOCC();
    dtCurve * result( void );
  private:
    ptrHandling< dtCurve > _dtC;
  };
}
#endif	/* geomCurve_baseConstructOCC_H */

