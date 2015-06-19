#ifndef bSplineSurfaces_bSplineSurfaceSkinConstructOCC_H
#define	bSplineSurfaces_bSplineSurfaceSkinConstructOCC_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/vectorHandling.h>

namespace dtOO {
  class dtSurface;
  
  class bSplineSurfaces_bSplineSurfaceSkinConstructOCC {
  public:
    dt__classOnlyName(bSplineSurfaces_bSplineSurfaceSkinConstructOCC);    
    bSplineSurfaces_bSplineSurfaceSkinConstructOCC();
    bSplineSurfaces_bSplineSurfaceSkinConstructOCC( 
      dt__vH(dtSurface const *) const & dtS 
    );
    virtual ~bSplineSurfaces_bSplineSurfaceSkinConstructOCC();
    vectorHandling< dtSurface * > result( void );    
  private:
    dt__vH(dtSurface *) _dtS;
  };
}
#endif	/* bSplineSurfaces_bSplineSurfaceSkinConstructOCC_H */

