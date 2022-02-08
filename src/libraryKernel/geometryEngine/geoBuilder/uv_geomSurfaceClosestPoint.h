#ifndef uv_geomSurfaceClosestPoint_H
#define	uv_geomSurfaceClosestPoint_H

#include <dtOOTypeDef.h>

#include <logMe/dtMacros.h>
#include <dtLinearAlgebra.h>
#include <vector>

namespace dtOO {
  class dtSurface;
  
  class uv_geomSurfaceClosestPoint {
  public:
    dt__classOnlyName(uv_geomSurfaceClosestPoint);     
    uv_geomSurfaceClosestPoint(
      dtSurface const * const dtS, dtPoint3 const & pXYZ
    );
    virtual ~uv_geomSurfaceClosestPoint();
    dtPoint2 result( void );
  private:
    double F(double const * xx) const;    
  private:
    dtPoint2 _closestUV;
    dtSurface const & _dtS;
    dtPoint3 const & _pXYZ;
  };
}
#endif	/* uv_geomSurfaceClosestPoint_H */

