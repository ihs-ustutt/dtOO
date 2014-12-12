#ifndef GEOMCURVE2D_IGNOREZ_H
#define	GEOMCURVE2D_IGNOREZ_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/ptrHandling.h>

namespace dtOO {
  class dtCurve;
  class dtCurve2d;
  
  class geomCurve2d_ignoreZ {
  public:
    dt__CLASSNAME(geomCurve2d_ignoreZ);
    geomCurve2d_ignoreZ( dtCurve const * const dtC );
    virtual ~geomCurve2d_ignoreZ();
    dtCurve2d * result( void );
  private:
    ptrHandling<dtCurve2d> _dtC2d;
  };
}
#endif	/* GEOMCURVE2D_IGNOREZ_H */

