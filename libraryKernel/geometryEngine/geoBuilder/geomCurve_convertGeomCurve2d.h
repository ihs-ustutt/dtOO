#ifndef GEOMCURVE_CONVERTGEOMCURVE2D_H
#define	GEOMCURVE_CONVERTGEOMCURVE2D_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/ptrHandling.h>

namespace dtOO {
  class dtCurve;
  class dtCurve2d;
  
  class geomCurve_convertGeomCurve2d {
  public:
    dt__CLASSNAME(geomCurve_convertGeomCurve2d);
    geomCurve_convertGeomCurve2d( dtCurve2d const * const dtC2d, dtVector3 const & vv );
    virtual ~geomCurve_convertGeomCurve2d();
    dtCurve * result( void );
  private:
    ptrHandling<dtCurve> _dtC;
  };
}
#endif	/* GEOMCURVE_CONVERTGEOMCURVE2D_H */

