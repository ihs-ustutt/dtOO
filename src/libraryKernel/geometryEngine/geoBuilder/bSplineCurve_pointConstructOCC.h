#ifndef bSplineCurve_pointConstructOCC_H
#define	bSplineCurve_pointConstructOCC_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/ptrHandling.h>

namespace dtOO {
  class dtCurve;
  
  class bSplineCurve_pointConstructOCC {
  public:
    dt__classOnlyName(bSplineCurve_pointConstructOCC);
    bSplineCurve_pointConstructOCC( 
      std::vector< dtPoint3 > const & pp, int const & order 
    );
    bSplineCurve_pointConstructOCC( dtPoint3 const & p0, dtPoint3 const & p1 );    
    virtual ~bSplineCurve_pointConstructOCC();
    dtCurve * result( void );
  private:
    dt__pH(dtCurve) _dtC;
  };
}
#endif	/* bSplineCurve_pointConstructOCC_H */

