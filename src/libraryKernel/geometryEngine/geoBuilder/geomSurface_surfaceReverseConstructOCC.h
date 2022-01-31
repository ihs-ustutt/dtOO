#ifndef geomSurface_surfaceReverseConstructOCC_H
#define	geomSurface_surfaceReverseConstructOCC_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/ptrHandling.h>
#include <interfaceHeaven/vectorHandling.h>

namespace dtOO {
  class dtSurface;
  
  class geomSurface_surfaceReverseConstructOCC {
  public:
    dt__classOnlyName(geomSurface_surfaceReverseConstructOCC);    
    geomSurface_surfaceReverseConstructOCC();
    geomSurface_surfaceReverseConstructOCC(
      dtSurface const * const dtS, bool const & revU, bool const & revV
    );
    virtual ~geomSurface_surfaceReverseConstructOCC();
    dtSurface * result( void );    
  private:
    ptrHandling< dtSurface > _dtS;
  };
}
#endif	/* geomSurface_surfaceReverseConstructOCC_H */

