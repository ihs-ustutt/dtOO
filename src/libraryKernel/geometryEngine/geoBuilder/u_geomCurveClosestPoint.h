#ifndef u_geomCurveClosestPoint_H
#define	u_geomCurveClosestPoint_H

#include <logMe/dtMacros.h>
#include <dtLinearAlgebra.h>
#include <vector>

namespace dtOO {
  class dtCurve;
  
  class u_geomCurveClosestPoint {
    public:
      dt__classOnlyName(u_geomCurveClosestPoint);     
      u_geomCurveClosestPoint(
        dtCurve const * const dtC, dtPoint3 const & pXYZ
      );
      virtual ~u_geomCurveClosestPoint();
      float result( void );
    private:
      double F(double const * xx) const;    
    private:
      float _closestU;
      dtCurve const & _dtC;
      dtPoint3 const & _pXYZ;
  };
}
#endif	/* u_geomCurveClosestPoint_H */

