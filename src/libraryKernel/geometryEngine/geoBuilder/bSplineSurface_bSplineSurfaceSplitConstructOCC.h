#ifndef bSplineSurface_bSplineSurfaceSplitConstructOCC_H
#define	bSplineSurface_bSplineSurfaceSplitConstructOCC_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>

namespace dtOO {
  class dtSurface;
  
  class bSplineSurface_bSplineSurfaceSplitConstructOCC {
    public:
      dt__classOnlyName(bSplineSurface_bSplineSurfaceSplitConstructOCC);
      bSplineSurface_bSplineSurfaceSplitConstructOCC( 
        dtSurface const * const dtS, 
        dtInt const & dir, dtReal const & p0, dtReal const & p1
      );
      virtual ~bSplineSurface_bSplineSurfaceSplitConstructOCC();
      dtSurface * result( void );
    private:
      dt__pH(dtSurface) _dtS;
  };
}
#endif	/* bSplineSurface_bSplineSurfaceSplitConstructOCC_H */

