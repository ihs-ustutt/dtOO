#ifndef bSplineSurface_convertOCC_H
#define	bSplineSurface_convertOCC_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/ptrHandling.h>
#include <interfaceHeaven/vectorHandling.h>

namespace dtOO {
  class dtSurface;
  
  class bSplineSurface_convertOCC {
  public:
    dt__classOnlyName(bSplineSurface_convertOCC);
    bSplineSurface_convertOCC( dtSurface const * const dtS );
    virtual ~bSplineSurface_convertOCC();
    dtSurface * result( void );
  private:
    ptrHandling< dtSurface > _dtS;
  };
}
#endif	/* bSplineSurface_convertOCC_H */

