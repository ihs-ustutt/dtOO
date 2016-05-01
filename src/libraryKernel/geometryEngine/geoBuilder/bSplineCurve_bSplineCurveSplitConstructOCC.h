#ifndef bSplineCurve_bSplineCurveSplitConstructOCC_H
#define	bSplineCurve_bSplineCurveSplitConstructOCC_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>

namespace dtOO {
  class dtCurve;
  
  class bSplineCurve_bSplineCurveSplitConstructOCC {
  public:
    dt__classOnlyName(bSplineCurve_bSplineCurveSplitConstructOCC);
    bSplineCurve_bSplineCurveSplitConstructOCC( 
      dtCurve const * const dtC, float const & u0, float const & u1
    );
    virtual ~bSplineCurve_bSplineCurveSplitConstructOCC();
    dtCurve * result( void );
  private:
    dt__pH(dtCurve) _dtC;
  };
}
#endif	/* bSplineCurve_bSplineCurveSplitConstructOCC_H */

