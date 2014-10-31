#ifndef RECTANGULARTRIMMEDSURFACE_CURVEROTATECONSTRUCTOCC_H
#define	RECTANGULARTRIMMEDSURFACE_CURVEROTATECONSTRUCTOCC_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/ptrHandling.h>

namespace dtOO {
  class dtSurface;
  class dtCurve;
  
  class rectangularTrimmedSurface_curveRotateConstructOCC {
  public:
    dt__CLASSNAME(rectangularTrimmedSurface_curveRotateConstructOCC);  
    rectangularTrimmedSurface_curveRotateConstructOCC( 
      dtCurve const & cL, 
      dtPoint3 const & oP, 					
      dtVector3 const & rA, 
      float const & angle 
    );
    virtual ~rectangularTrimmedSurface_curveRotateConstructOCC();
    dtSurface * result( void );
  private:
    ptrHandling<dtSurface> _dtS;
  };
}
#endif	/* RECTANGULARTRIMMEDSURFACE_CURVEROTATECONSTRUCTOCC_H */

