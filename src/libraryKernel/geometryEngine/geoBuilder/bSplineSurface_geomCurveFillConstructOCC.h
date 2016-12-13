#ifndef bSplineSurface_geomCurveFillConstructOCC_H
#define	bSplineSurface_geomCurveFillConstructOCC_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/vectorHandling.h>

namespace dtOO {
  class dtCurve;
  class dtSurface;
  
  class bSplineSurface_geomCurveFillConstructOCC {
    public:
      dt__classOnlyName(bSplineSurface_geomCurveFillConstructOCC);
      bSplineSurface_geomCurveFillConstructOCC( 
        vectorHandling< dtCurve const * > const & cc 
      );
      bSplineSurface_geomCurveFillConstructOCC( dt__pVH(dtCurve) const & cc );      
      bSplineSurface_geomCurveFillConstructOCC( 
        dtCurve const * c0, dtCurve const * c1, 
        dtCurve const * c2, dtCurve const * c3 
      );
      void init( vectorHandling< dtCurve const * > const & cc );
      virtual ~bSplineSurface_geomCurveFillConstructOCC();
      dtSurface * result( void );
    private:
      ptrHandling<dtSurface> _dtS;
  };
}
#endif	/* bSplineSurface_geomCurveFillConstructOCC_H */

