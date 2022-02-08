#ifndef bSplineCurve_approxGeomCurve2dInGeomSurface_H
#define	bSplineCurve_approxGeomCurve2dInGeomSurface_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/ptrHandling.h>

namespace dtOO {
  class dtCurve;
  class dtCurve2d;
  class dtSurface;
  
  class bSplineCurve_approxGeomCurve2dInGeomSurface {
  public:
    dt__classOnlyName(bSplineCurve_approxGeomCurve2dInGeomSurface);
    bSplineCurve_approxGeomCurve2dInGeomSurface( 
      dtCurve2d const * const dtC2d, dtSurface const * const dtS
    );
    virtual ~bSplineCurve_approxGeomCurve2dInGeomSurface();
    dtCurve * result( void );
  private:
    dt__pH(dtCurve) _dtC;
  };
}
#endif	/* bSplineCurve_approxGeomCurve2dInGeomSurface_H */

