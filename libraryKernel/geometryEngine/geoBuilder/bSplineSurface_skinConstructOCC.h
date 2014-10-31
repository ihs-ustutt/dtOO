#ifndef BSPLINESURFACE_SKINCONSTRUCTOCC_H
#define	BSPLINESURFACE_SKINCONSTRUCTOCC_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/ptrHandling.h>
#include <interfaceHeaven/vectorHandling.h>

namespace dtOO {
  class dtSurface;
  class dtCurve;
  
  class bSplineSurface_skinConstructOCC {
  public:
    dt__CLASSNAME(bSplineSurface_skinConstructOCC);
    bSplineSurface_skinConstructOCC();
    bSplineSurface_skinConstructOCC(vectorHandling< dtCurve const * > const &  cL);
    virtual ~bSplineSurface_skinConstructOCC();
    dtSurface * result( void );
  private:
    ptrHandling<dtSurface> _dtS;
  };
}
#endif	/* BSPLINESURFACE_SKINCONSTRUCTOCC_H */

