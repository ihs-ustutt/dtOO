#ifndef bSplineSurfaces_bSplineSurfaceSkinConstructOCC_H
#define	bSplineSurfaces_bSplineSurfaceSkinConstructOCC_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/vectorHandling.h>

namespace dtOO {
  class dtSurface;
  
  class bSplineSurfaces_bSplineSurfaceSkinConstructOCC {
  public:
    dt__CLASSNAME(bSplineSurfaces_bSplineSurfaceSkinConstructOCC);    
    bSplineSurfaces_bSplineSurfaceSkinConstructOCC();
    bSplineSurfaces_bSplineSurfaceSkinConstructOCC( vectorHandling< dtSurface const * > const & dtS );
    virtual ~bSplineSurfaces_bSplineSurfaceSkinConstructOCC();
    vectorHandling< dtSurface * > result( void );    
  private:
    vectorHandling< dtSurface * > _dtS;
  };
}
#endif	/* bSplineSurfaces_bSplineSurfaceSkinConstructOCC_H */

