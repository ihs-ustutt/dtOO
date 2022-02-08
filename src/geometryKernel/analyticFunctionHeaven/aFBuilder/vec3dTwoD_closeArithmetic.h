#ifndef vec3dTwoD_closeArithmetic_H
#define	vec3dTwoD_closeArithmetic_H

#include <dtOOTypeDef.h>

#include <logMe/dtMacros.h>
#include <dtLinearAlgebra.h>

namespace dtOO {
  class vec3dTwoD;
  
  class vec3dTwoD_closeArithmetic {
  public:
    dt__classOnlyName(vec3dTwoD_closeArithmetic);    
    vec3dTwoD_closeArithmetic( 
      dt__pVH(vec3dTwoD) const & v3d, dtInt const & dir, dtInt const & nSegments
    );
    virtual ~vec3dTwoD_closeArithmetic();
    dt__pVH(vec3dTwoD) result(void);
  private:
    dt__pVH(vec3dTwoD) _v3d;
  };
}
#endif	/* vec3dTwoD_closeArithmetic_H */

