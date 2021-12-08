#ifndef bSplineSurface_constrainedFillingConstructOCC_H
#define	bSplineSurface_constrainedFillingConstructOCC_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/vectorHandling.h>

namespace dtOO {
  class dtCurve;
  class dtSurface;
  
  class bSplineSurface_constrainedFillingConstructOCC {
    public:
      dt__classOnlyName(bSplineSurface_constrainedFillingConstructOCC);

      bSplineSurface_constrainedFillingConstructOCC( 
        dtCurve const * c0, dtCurve const * c1, 
        dtCurve const * c2, dtCurve const * c3,
        int const & maxDeg, int const & maxSeg
      );
      virtual ~bSplineSurface_constrainedFillingConstructOCC();
      dtSurface * result( void );
    private:
      ptrHandling<dtSurface> _dtS;
  };
}
#endif	/* bSplineSurface_constrainedFillingConstructOCC_H */

