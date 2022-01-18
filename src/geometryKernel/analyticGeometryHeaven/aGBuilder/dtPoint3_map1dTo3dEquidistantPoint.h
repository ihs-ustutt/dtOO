#ifndef dtPoint3_map1dTo3dEquidistantPoint_H
#define	dtPoint3_map1dTo3dEquidistantPoint_H

#include <dtOOTypeDef.h>

#include <logMe/dtMacros.h>
#include <dtLinearAlgebra.h>
#include <vector>

namespace dtOO {
  class map1dTo3d;
  
  class dtPoint3_map1dTo3dEquidistantPoint {
  public:
    dt__classOnlyName(dtPoint3_map1dTo3dEquidistantPoint);     
    dtPoint3_map1dTo3dEquidistantPoint(
      map1dTo3d const * const m1d, dtInt const & nPoints
    );
    virtual ~dtPoint3_map1dTo3dEquidistantPoint();
    std::vector< dtPoint3 > result( void );
  private:
    std::vector< dtPoint3 > _ppXYZ;
  };
}
#endif	/* dtPoint3_map1dTo3dEquidistantPoint_H */

