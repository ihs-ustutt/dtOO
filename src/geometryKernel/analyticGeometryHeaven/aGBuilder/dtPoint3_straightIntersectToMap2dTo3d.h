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

