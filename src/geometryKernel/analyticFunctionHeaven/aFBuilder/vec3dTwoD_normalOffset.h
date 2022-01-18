#ifndef vec3dTwoD_normalOffset_H
#define	vec3dTwoD_normalOffset_H

#include <dtOOTypeDef.h>

#include <logMe/dtMacros.h>
#include <dtLinearAlgebra.h>

namespace dtOO {
  class vec3dTwoD;
  class vec3dOneD;
  
  class vec3dTwoD_normalOffset {
    public:
      dt__classOnlyName(vec3dTwoD_normalOffset);    
      vec3dTwoD_normalOffset(
        vec3dTwoD const * const v3d, vec3dTwoD const * const thick,
        dtVector3 const & nf, dtInt const & nU, dtInt const & nV,
        dtInt const & order,
        dtInt const & skinOrderMin, dtInt const & skinOrderMax,
        dtInt const & skinNIterations,
        bool const & closeU = false,
        dtReal const & closeSmooth = -1.
      );
      virtual ~vec3dTwoD_normalOffset();
      vec3dTwoD * result(void) const;
      dt__pVH(vec3dOneD) resultWire(void) const;
    private:
      dt__pH(vec3dTwoD) _vS;
      dt__pVH(vec3dOneD) _vC;    
  };
}
#endif	/* vec3dTwoD_normalOffset_H */

