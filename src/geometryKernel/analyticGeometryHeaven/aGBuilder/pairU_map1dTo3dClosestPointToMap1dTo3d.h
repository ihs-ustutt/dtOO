#ifndef pairU_map1dTo3dClosestPointToMap1dTo3d_H
#define	pairU_map1dTo3dClosestPointToMap1dTo3d_H

#include <dtOOTypeDef.h>

#include <logMe/dtMacros.h>
#include <dtLinearAlgebra.h>
#include <vector>

namespace dtOO {
  class map1dTo3d;
  
  class pairU_map1dTo3dClosestPointToMap1dTo3d {
  public:
    dt__classOnlyName(pairU_map1dTo3dClosestPointToMap1dTo3d);     
    pairU_map1dTo3dClosestPointToMap1dTo3d(
      map1dTo3d const * const m1d0, map1dTo3d const * const m1d1
    );
    virtual ~pairU_map1dTo3dClosestPointToMap1dTo3d();
    std::pair< dtReal, dtReal > result( void );
  private:
    double F(double const * xx) const;    
  private:
    std::pair< dtReal, dtReal > _closestU;
    map1dTo3d const & _m1d0;
    map1dTo3d const & _m1d1;
  };
}
#endif	/* pairU_map1dTo3dClosestPointToMap1dTo3d_H */

