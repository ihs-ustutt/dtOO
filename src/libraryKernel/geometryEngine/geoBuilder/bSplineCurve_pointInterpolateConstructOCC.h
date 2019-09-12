#ifndef bSplineCurve_pointInterpolateConstructOCC_H
#define	bSplineCurve_pointInterpolateConstructOCC_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>

namespace dtOO {
  class dtCurve;
  
  class bSplineCurve_pointInterpolateConstructOCC {
  public:
    dt__classOnlyName(bSplineCurve_pointInterpolateConstructOCC);
    bSplineCurve_pointInterpolateConstructOCC( 
      std::vector < dtPoint3 > const & pp 
    );
    bSplineCurve_pointInterpolateConstructOCC( 
      std::vector < dtPoint3 > const & pp, 
      int const & degMin, int const & degMax
    );
    bSplineCurve_pointInterpolateConstructOCC( 
      std::vector < dtPoint3 > const & pp, 
      int const & degMin, int const & degMax,
      int const & cont
    );
    virtual ~bSplineCurve_pointInterpolateConstructOCC();
    dtCurve * result( void );
  private:
    dt__pH(dtCurve) _dtC;
  };
}
#endif	/* bSplineCurve_pointInterpolateConstructOCC_H */

