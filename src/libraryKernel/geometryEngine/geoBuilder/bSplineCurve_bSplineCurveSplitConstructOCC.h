#ifndef bSplineCurve_bSplineCurveSplitConstructOCC_H
#define	bSplineCurve_bSplineCurveSplitConstructOCC_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>

namespace dtOO {
  class dtCurve;
  
  class bSplineCurve_bSplineCurveSplitConstructOCC {
  public:
    dt__classOnlyName(bSplineCurve_bSplineCurveSplitConstructOCC);
    bSplineCurve_bSplineCurveSplitConstructOCC( 
      dtCurve const * const dtC, dtReal const & u0, dtReal const & u1
    );
    virtual ~bSplineCurve_bSplineCurveSplitConstructOCC();
    dtCurve * result( void );
  private:
    dt__pH(dtCurve) _dtC;
  };
}
#endif	/* bSplineCurve_bSplineCurveSplitConstructOCC_H */

