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

#ifndef dtPoint3_straightIntersectToMap2dTo3d_H
#define	dtPoint3_straightIntersectToMap2dTo3d_H

#include <dtOOTypeDef.h>

#include <logMe/dtMacros.h>
#include <dtLinearAlgebra.h>

namespace dtOO {
  class map2dTo3d;
  
  class dtPoint3_straightIntersectToMap2dTo3d {
    public:
      dt__classOnlyName(dtPoint3_straightIntersectToMap2dTo3d);         
      dtPoint3_straightIntersectToMap2dTo3d(
        dtPoint3 const & p0, dtVector3 const & v0, map2dTo3d const * const m2d
      );
      virtual ~dtPoint3_straightIntersectToMap2dTo3d();
      dtPoint3 result( void );
    private:
      double F(double const * xx) const;    
    private:
      dtPoint3 _pp;
      dtPoint3 _p0;
      dtVector3 _v0;
      map2dTo3d const * _m2d;
  };
}
#endif	/* dtPoint3_straightIntersectToMap2dTo3d_H */
