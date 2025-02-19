/*---------------------------------------------------------------------------*\
  dtOO < design tool Object-Oriented >
    
    Copyright (C) 2024 A. Tismer.
-------------------------------------------------------------------------------
License
    This file is part of dtOO.

    dtOO is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the LICENSE.txt file in the
    dtOO root directory for more details.

    You should have received a copy of the License along with dtOO.

\*---------------------------------------------------------------------------*/

#ifndef u_geomCurveClosestPoint_H
#define	u_geomCurveClosestPoint_H

#include <dtOOTypeDef.h>

#include <logMe/dtMacros.h>
#include <dtLinearAlgebra.h>

namespace dtOO {
  class dtCurve;
  
  class u_geomCurveClosestPoint {
    public:
      dt__classOnlyName(u_geomCurveClosestPoint);     
      u_geomCurveClosestPoint(
        dtCurve const * const dtC, dtPoint3 const & pXYZ
      );
      virtual ~u_geomCurveClosestPoint();
      dtReal result( void );
    private:
      double F(double const * xx) const;    
    private:
      dtReal _closestU;
      dtCurve const & _dtC;
      dtPoint3 const & _pXYZ;
  };
}
#endif	/* u_geomCurveClosestPoint_H */
