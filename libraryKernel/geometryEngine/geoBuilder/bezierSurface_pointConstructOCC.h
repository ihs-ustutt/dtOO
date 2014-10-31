#ifndef BEZIERSURFACEPOINTCONSTRUCTOCC_H
#define	BEZIERSURFACEPOINTCONSTRUCTOCC_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/ptrHandling.h>
#include <interfaceHeaven/twoDArrayHandling.h>

namespace dtOO {
  class dtSurface;
  
  class bezierSurface_pointConstructOCC {
  public:
    dt__CLASSNAME(bezierSurface_pointConstructOCC);
    bezierSurface_pointConstructOCC( twoDArrayHandling< dtPoint3 > const & pp );
    virtual ~bezierSurface_pointConstructOCC();
    dtSurface * result( void );
  private:
    ptrHandling<dtSurface> _dtS;
  };
}
#endif	/* BEZIERSURFACEPOINTCONSTRUCTOCC_H */

