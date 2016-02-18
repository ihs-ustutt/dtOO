#ifndef uv_map2dTo3dClosestPointToPoint_H
#define	uv_map2dTo3dClosestPointToPoint_H

#include <logMe/dtMacros.h>
#include <dtLinearAlgebra.h>
#include <vector>

namespace dtOO {
  class map2dTo3d;
  
  class uv_map2dTo3dClosestPointToPoint {
  public:
    dt__classOnlyName(uv_map2dTo3dClosestPointToPoint);     
    uv_map2dTo3dClosestPointToPoint(
      map2dTo3d const * const m2d, dtPoint3 const & pXYZ
    );
    virtual ~uv_map2dTo3dClosestPointToPoint();
    dtPoint2 result( void );
  private:
    double F(double const * xx) const;    
  private:
    dtPoint2 _closestUV;
    map2dTo3d const & _m2d;
    dtPoint3 const & _pXYZ;
  };
}
#endif	/* uv_map2dTo3dClosestPointToPoint_H */

