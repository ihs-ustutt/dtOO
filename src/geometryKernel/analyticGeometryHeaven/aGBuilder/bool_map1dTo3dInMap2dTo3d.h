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

#ifndef bool_map1dTo3dInMap2dTo3d_H
#define	bool_map1dTo3dInMap2dTo3d_H

#include <dtOOTypeDef.h>

#include <logMe/dtMacros.h>
#include <dtLinearAlgebra.h>
#include <vector>

namespace dtOO {
  class map1dTo3d;
  class map2dTo3d;
  
  class bool_map1dTo3dInMap2dTo3d {
    public:
      dt__classOnlyName(bool_map1dTo3dInMap2dTo3d);     
      bool_map1dTo3dInMap2dTo3d(
        map1dTo3d const * const m1d, map2dTo3d const * const m2d
      );
      virtual ~bool_map1dTo3dInMap2dTo3d();
      bool result( void );
      std::pair< dtPoint2, dtPoint2 > points(void) const;
    private:
      std::pair< dtPoint2, dtPoint2 > _points;
      bool _inside;
  };
}
#endif	/* bool_map1dTo3dInMap2dTo3d_H */
