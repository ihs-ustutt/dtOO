#ifndef TRIMMEDCURVE_TWOPOINTSCONNECTCONSTRUCT_H
#define	TRIMMEDCURVE_TWOPOINTSCONNECTCONSTRUCT_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/ptrHandling.h>

namespace dtOO {
  class dtCurve;
  
  class trimmedCurve_twoPointsConnectConstructOCC {
  public:
    dt__CLASSNAME(trimmedCurve_twoPointsConnectConstructOCC);
    trimmedCurve_twoPointsConnectConstructOCC( dtPoint3 const & p0, dtPoint3 const & p1 );
    virtual ~trimmedCurve_twoPointsConnectConstructOCC();
    dtCurve * result( void );
  private:
    ptrHandling<dtCurve> _dtC;
  };
}
#endif	/* TRIMMEDCURVE_TWOPOINTSCONNECTCONSTRUCT_H */

