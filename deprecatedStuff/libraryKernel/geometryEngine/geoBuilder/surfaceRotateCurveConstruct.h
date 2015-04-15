#ifndef SURFACEROTATECURVECONSTRUCT_H
#define	SURFACEROTATECURVECONSTRUCT_H

#include <dtLinearAlgebra.h>
#include <interfaceHeaven/ptrHandling.h>
#include <math.h>

namespace dtOO {
  class dtSurface;
  class dtCurve;
  
  class surfaceRotateCurveConstruct {
  public:
    dt__classOnlyName(surfaceRotateCurveConstruct);     
    surfaceRotateCurveConstruct(
      dtPoint3 const & origin, 
      dtVector3 const & rotAxis, 
      dtCurve const & curve, 
      float const & angle = 2. * M_PI
    );
    virtual ~surfaceRotateCurveConstruct();
    dtSurface * result( void );
  private:
    ptrHandling<dtSurface> _dtS;
  };
}
#endif	/* SURFACEROTATECURVECONSTRUCT_H */

