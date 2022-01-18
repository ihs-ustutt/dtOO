#ifndef geomSurface_closeGapsArithmeticOCC_H
#define	geomSurface_closeGapsArithmeticOCC_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/ptrHandling.h>
#include <interfaceHeaven/vectorHandling.h>

namespace dtOO {
  class dtSurface;
  
  class geomSurface_closeGapsArithmeticOCC {
  public:
    dt__classOnlyName(geomSurface_closeGapsArithmeticOCC);    
    geomSurface_closeGapsArithmeticOCC();
    geomSurface_closeGapsArithmeticOCC(
      dt__pVH(dtSurface) const & dtS, dtInt const & dir, dtInt const & nSegments
    );
    virtual ~geomSurface_closeGapsArithmeticOCC();
    dt__pVH(dtSurface) result( void );    
  private:
    dt__pVH(dtSurface) _dtS;
  };
}
#endif	/* geomSurface_closeGapsArithmeticOCC_H */

