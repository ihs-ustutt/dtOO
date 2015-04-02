#ifndef SURFACEOFREVOLUTION_CURVEROTATECONSTRUCTOCC_H
#define	SURFACEOFREVOLUTION_CURVEROTATECONSTRUCTOCC_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/ptrHandling.h>

namespace dtOO {
  class dtSurface;
  class dtCurve;
  
  class surfaceOfRevolution_curveRotateConstructOCC {
  public:
    dt__classOnlyName(surfaceOfRevolution_curveRotateConstructOCC);  
    surfaceOfRevolution_curveRotateConstructOCC();
    surfaceOfRevolution_curveRotateConstructOCC( 
      dtCurve const & cL, 
      dtPoint3 const & oP, 					
      dtVector3 const & rA
    );
    virtual ~surfaceOfRevolution_curveRotateConstructOCC();
    dtSurface * result( void );
  private:
    ptrHandling<dtSurface> _dtS;
  };
}
#endif	/* SURFACEOFREVOLUTION_CURVEROTATECONSTRUCTOCC_H */

