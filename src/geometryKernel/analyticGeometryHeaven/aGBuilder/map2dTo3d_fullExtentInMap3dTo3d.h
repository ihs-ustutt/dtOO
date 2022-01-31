#ifndef map2dTo3d_fullExtentInMap3dTo3d_H
#define	map2dTo3d_fullExtentInMap3dTo3d_H

#include <dtOOTypeDef.h>

#include <logMe/dtMacros.h>

namespace dtOO {
  class map2dTo3d;
  
  class map2dTo3d_fullExtentInMap3dTo3d {
  public:
    dt__classOnlyName(map2dTo3d_fullExtentInMap3dTo3d);
    map2dTo3d_fullExtentInMap3dTo3d(
      map2dTo3d const * const m2d, dtInt const & m2dParDir, dtInt const & toM3dParDir
    );
    virtual ~map2dTo3d_fullExtentInMap3dTo3d();
    map2dTo3d * result( void );
  private:
    dt__pH(map2dTo3d) _m2d;
  };
}
#endif	/* map2dTo3d_fullExtentInMap3dTo3d_H */

