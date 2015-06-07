#ifndef vec3dTwoD_normalOffset_H
#define	vec3dTwoD_normalOffset_H

#include <logMe/dtMacros.h>
#include <dtLinearAlgebra.h>
#include <vector>

namespace dtOO {
  class vec3dTwoD;
//  class vec3dSurfaceTwoD;
  
  class vec3dTwoD_normalOffset {
  public:
    dt__classOnlyName(vec3dTwoD_normalOffset);    
    vec3dTwoD_normalOffset(
      vec3dTwoD const * const v3d, vec3dTwoD const * const thick,
      dtVector3 const & nf, int const & nU, int const & nV,
      int const & order      
    );
    virtual ~vec3dTwoD_normalOffset();
     vec3dTwoD * result(void);
  private:
    dt__pH(vec3dTwoD) _v3d;
  };
}
#endif	/* vec3dTwoD_normalOffset_H */

