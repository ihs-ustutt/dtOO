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
    dt__classOnlyName(bSplineSurface_skinConstructOCC);
    bSplineSurface_skinConstructOCC();
    bSplineSurface_skinConstructOCC(
    	vectorHandling< dtCurve const * > const &  cL
    );    
    bSplineSurface_skinConstructOCC(
    	vectorHandling< dtCurve const * > const &  cL, 
      int const & minDeg, int const & maxDeg, int const & nIter
    );
    virtual ~bSplineSurface_skinConstructOCC();
    dtSurface * result( void );
  private:
    ptrHandling<dtSurface> _dtS;
  };
}
#endif	/* BSPLINESURFACE_SKINCONSTRUCTOCC_H */
