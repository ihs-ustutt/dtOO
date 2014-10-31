#ifndef CURVEPOINTCONSTRUCT_H
#define	CURVEPOINTCONSTRUCT_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/ptrHandling.h>

namespace dtOO {
  class dtCurve;
  
  class curvePointConstruct {
  public:
    dt__CLASSNAME(curvePointConstruct);
    curvePointConstruct( std::vector < dtPoint3 > const & pp, int const & order );
    virtual ~curvePointConstruct();
    dtCurve * result( void );
  private:
    ptrHandling<dtCurve> _dtC;
  };
}
#endif	/* CURVEPOINTCONSTRUCT_H */

