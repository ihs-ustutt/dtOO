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

