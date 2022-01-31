#ifndef geomCurve2d_baseConstructOCC_H
#define	geomCurve2d_baseConstructOCC_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/ptrHandling.h>
#include <interfaceHeaven/vectorHandling.h>

namespace dtOO {
  class dtCurve2d;
  class dtOCCCurve2dBase;
  
  class geomCurve2d_baseConstructOCC {
  public:
    dt__classOnlyName(geomCurve2d_baseConstructOCC);
    geomCurve2d_baseConstructOCC( dtOCCCurve2dBase const & base );
    virtual ~geomCurve2d_baseConstructOCC();
    dtCurve2d * result( void );
  private:
    ptrHandling< dtCurve2d > _dtC2d;
  };
}
#endif	/* geomCurve2d_baseConstructOCC_H */

