#ifndef geomSurface_baseConstructOCC_H
#define	geomSurface_baseConstructOCC_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/ptrHandling.h>
#include <interfaceHeaven/vectorHandling.h>

namespace dtOO {
  class dtSurface;
  class dtOCCSurfaceBase;
  
  class geomSurface_baseConstructOCC {
  public:
    dt__classOnlyName(geomSurface_baseConstructOCC);
    geomSurface_baseConstructOCC( dtOCCSurfaceBase const & base );
    virtual ~geomSurface_baseConstructOCC();
    static bool support( dtOCCSurfaceBase const & base );
    dtSurface * result( void );
  private:
    ptrHandling<dtSurface> _dtS;
  };
}
#endif	/* geomSurface_baseConstructOCC_H */
