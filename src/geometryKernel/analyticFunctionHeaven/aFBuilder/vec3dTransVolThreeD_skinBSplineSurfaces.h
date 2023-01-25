#ifndef vec3dTransVolThreeD_skinBSplineSurfaces_H
#define	vec3dTransVolThreeD_skinBSplineSurfaces_H

#include <dtOOTypeDef.h>

#include <logMe/dtMacros.h>
#include <dtLinearAlgebra.h>

namespace dtOO {
  template< typename T > class vectorHandling;
  class analyticFunction;
  class vec3dTransVolThreeD;
  
  class vec3dTransVolThreeD_skinBSplineSurfaces {
    public:
      dt__classOnlyName(vec3dTransVolThreeD_skinBSplineSurfaces);    
      vec3dTransVolThreeD_skinBSplineSurfaces(
        vectorHandling< analyticFunction const * > const * const aF
      );
      virtual ~vec3dTransVolThreeD_skinBSplineSurfaces();
      vec3dTransVolThreeD * result(void) const;
    private:
      dt__pH(vec3dTransVolThreeD) _vT;
  };
}
#endif	/* vec3dTransVolThreeD_skinBSplineSurfaces_H */

