#ifndef x_vec3dClosestPointToPoint_H
#define	x_vec3dClosestPointToPoint_H

#include <dtOOTypeDef.h>

#include <logMe/dtMacros.h>
#include <dtLinearAlgebra.h>
#include <vector>
#include <analyticFunctionHeaven/analyticFunction.h>

namespace dtOO {
  class vec3dFunction;
  
  class x_vec3dClosestPointToPoint {
  public:
    dt__classOnlyName(x_vec3dClosestPointToPoint);     
    x_vec3dClosestPointToPoint(
      vec3dFunction const * const v3d, aFY const & yy
    );
    virtual ~x_vec3dClosestPointToPoint();
    aFX result( void );
  private:
    double FOne(double const * xx) const;    
    double FTwo(double const * xx) const;    
  private:
    aFX _xx;
    vec3dFunction const * const _v3d;
    aFY const & _yy;
  };
}
#endif	/* x_vec3dClosestPointToPoint_H */
