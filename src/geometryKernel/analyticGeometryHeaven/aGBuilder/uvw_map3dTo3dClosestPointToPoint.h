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

#ifndef uvw_map3dTo3dClosestPointToPoint_H
#define	uvw_map3dTo3dClosestPointToPoint_H

#include <dtOOTypeDef.h>

#include <logMe/dtMacros.h>
#include <dtLinearAlgebra.h>
#include <vector>

namespace dtOO {
  class map3dTo3d;
  
  class uvw_map3dTo3dClosestPointToPoint {
    public:
      dt__classOnlyName(uvw_map3dTo3dClosestPointToPoint);     
      uvw_map3dTo3dClosestPointToPoint(
        map3dTo3d const * const m3d, dtPoint3 const & pXYZ
      );
      virtual ~uvw_map3dTo3dClosestPointToPoint();
      dtPoint3 result( void );
    private:
      double F(double const * xx) const;    
    private:
      dtPoint3 _closestUVW;
      map3dTo3d const & _m3d;
      dtPoint3 const & _pXYZ;
  };
}
#endif	/* uvw_map3dTo3dClosestPointToPoint_H */
