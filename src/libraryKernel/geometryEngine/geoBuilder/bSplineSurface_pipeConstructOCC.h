#ifndef BSPLINESURFACE_PIPECONSTRUCTOCC_H
#define	BSPLINESURFACE_PIPECONSTRUCTOCC_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/ptrHandling.h>

namespace dtOO {
  class dtCurve;
  class dtSurface;
  
  class bSplineSurface_pipeConstructOCC {
  public:
    dt__classOnlyName(bSplineSurface_pipeConstructOCC);
    bSplineSurface_pipeConstructOCC(
      dtCurve const * const aPath, 
      dtCurve const * const aSecOne, 
      dtCurve const * const aSecTwo            
    );
    bSplineSurface_pipeConstructOCC(
      dtCurve const * const aPath, dtCurve const * const aSecOne
    );    
    bSplineSurface_pipeConstructOCC(
      dtCurve const * const aPath, float const & rr
    );
    virtual ~bSplineSurface_pipeConstructOCC();
    dtSurface * result( void );
  private:
    dt__pH(dtSurface) _dtS;
  };
}
#endif	/* BSPLINESURFACE_PIPECONSTRUCTOCC_H */

