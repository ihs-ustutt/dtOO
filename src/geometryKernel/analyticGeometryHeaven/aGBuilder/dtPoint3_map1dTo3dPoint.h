#ifndef dtPoint3_map1dTo3dPoint_H
#define	dtPoint3_map1dTo3dPoint_H

#include <dtOOTypeDef.h>

#include <logMe/dtMacros.h>
#include <dtLinearAlgebra.h>
#include <vector>

namespace dtOO {
  class map1dTo3d;
  
  class dtPoint3_map1dTo3dPoint {
  public:
    dt__classOnlyName(dtPoint3_map1dTo3dPoint);     
    dtPoint3_map1dTo3dPoint(
      map1dTo3d const * const m1d, dtInt const & nPoints
    );
    virtual ~dtPoint3_map1dTo3dPoint();
    std::vector< dtPoint3 > result( void );
  private:
    std::vector< dtPoint3 > _ppXYZ;
  };
}
#endif	/* dtPoint3_map1dTo3dPoint_H */

