#ifndef BEZIERCURVEPOINTCONSTRUCTOCC_H
#define	BEZIERCURVEPOINTCONSTRUCTOCC_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/ptrHandling.h>

namespace dtOO {
  class dtCurve;
  
  class bezierCurve_pointConstructOCC {
  public:
    dt__CLASSNAME(bezierCurve_pointConstructOCC);
    bezierCurve_pointConstructOCC( std::vector < dtPoint3 > const & pp );
    virtual ~bezierCurve_pointConstructOCC();
    dtCurve * result( void );
  private:
    ptrHandling<dtCurve> _dtC;
  };
}
#endif	/* BEZIERCURVEPOINTCONSTRUCTOCC_H */

