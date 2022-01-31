#ifndef geomCurve2d_curve2dTranslateConstructOCC_H
#define	geomCurve2d_curve2dTranslateConstructOCC_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/ptrHandling.h>
#include <interfaceHeaven/vectorHandling.h>

namespace dtOO {
  class dtCurve2d;
  
  class geomCurve2d_curve2dTranslateConstructOCC {
  public:
    dt__classOnlyName(geomCurve2d_curve2dTranslateConstructOCC);    
    geomCurve2d_curve2dTranslateConstructOCC();
    geomCurve2d_curve2dTranslateConstructOCC(
      dtCurve2d const * const dtC2d, dtVector2 const & vv
    );
    virtual ~geomCurve2d_curve2dTranslateConstructOCC();
    dtCurve2d * result( void );    
  private:
    ptrHandling< dtCurve2d > _dtC2d;
  };
}
#endif	/* geomCurve2d_curve2dTranslateConstructOCC_H */

