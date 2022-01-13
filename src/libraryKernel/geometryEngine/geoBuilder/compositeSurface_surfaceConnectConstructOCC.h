#ifndef compositeSurface_surfaceConnectConstructOCC_H
#define	compositeSurface_surfaceConnectConstructOCC_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/twoDArrayHandling.h>

namespace dtOO {
  class dtSurface;
  
  class compositeSurface_surfaceConnectConstructOCC {
    public:
      dt__classOnlyName(compositeSurface_surfaceConnectConstructOCC);
      compositeSurface_surfaceConnectConstructOCC( 
        twoDArrayHandling< dtSurface const * > const & ss 
      );
      virtual ~compositeSurface_surfaceConnectConstructOCC();
      dtSurface * result( void );
    private:
      dt__pH(dtSurface) _dtS;
  };
}
#endif	/* BSPLINESURFACE_BSPLINECURVEFILLCONSTRUCTOCC_H */

