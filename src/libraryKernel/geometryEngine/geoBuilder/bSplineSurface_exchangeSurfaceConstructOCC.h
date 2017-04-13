#ifndef bSplineSurface_exchangeSurfaceConstructOCC_H
#define	bSplineSurface_exchangeSurfaceConstructOCC_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/ptrHandling.h>
#include <interfaceHeaven/vectorHandling.h>

namespace dtOO {
  class dtSurface;
  
  class bSplineSurface_exchangeSurfaceConstructOCC {
  public:
    dt__classOnlyName(bSplineSurface_exchangeSurfaceConstructOCC);
    bSplineSurface_exchangeSurfaceConstructOCC( 
      dtSurface const * const dtS
    );
    virtual ~bSplineSurface_exchangeSurfaceConstructOCC();
    dtSurface * result( void );
  private:
    ptrHandling< dtSurface > _dtS;
  };
}
#endif	/* bSplineSurface_exchangeSurfaceConstructOCC_H */

