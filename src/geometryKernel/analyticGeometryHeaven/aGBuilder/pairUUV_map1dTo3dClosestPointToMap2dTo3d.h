#ifndef pairUUV_map1dTo3dClosestPointToMap2dTo3d_H
#define	pairUUV_map1dTo3dClosestPointToMap2dTo3d_H

#include <dtOOTypeDef.h>

#include <logMe/dtMacros.h>
#include <dtLinearAlgebra.h>
#include <vector>

namespace dtOO {
  class map1dTo3d;
  class map2dTo3d;
  
  class pairUUV_map1dTo3dClosestPointToMap2dTo3d {
  public:
    dt__classOnlyName(pairUUV_map1dTo3dClosestPointToMap2dTo3d);     
    pairUUV_map1dTo3dClosestPointToMap2dTo3d(
      map1dTo3d const * const m1d, map2dTo3d const * const m2d
    );
    virtual ~pairUUV_map1dTo3dClosestPointToMap2dTo3d();
    std::pair< dtReal, dtPoint2 > result( void );
  private:
    double F(double const * xx) const;    
  private:
    std::pair< dtReal, dtPoint2 > _closestUUV;
    map1dTo3d const & _m1d;
    map2dTo3d const & _m2d;
  };
}
#endif	/* pairUUV_map1dTo3dClosestPointToMap2dTo3d_H */

