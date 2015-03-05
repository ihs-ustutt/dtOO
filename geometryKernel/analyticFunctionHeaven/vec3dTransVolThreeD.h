#ifndef VEC3DTRANSVOLTHREED_H
#define	VEC3DTRANSVOLTHREED_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include "vec3dThreeD.h"
#include "vec3dTwoD.h"
#include "analyticFunctionCompound.h"
#include <interfaceHeaven/ptrVectorHandling.h>

namespace dtOO {
  class vec3dTwoD;
  
  class vec3dTransVolThreeD : public vec3dThreeD {
  public:
    dt__CLASSSTD(vec3dTransVolThreeD, analyticFunction);     
    vec3dTransVolThreeD();
    vec3dTransVolThreeD(const vec3dTransVolThreeD& orig);
    vec3dTransVolThreeD(
      vec3dTwoD const * const v3d2d0,  
      vec3dTwoD const * const v3d2d1,
      vec3dTwoD const * const v3d2d2,
      vec3dTwoD const * const v3d2d3,
      vec3dTwoD const * const v3d2d4,
      vec3dTwoD const * const v3d2d5
    );
    virtual vec3dTransVolThreeD * clone( void ) const;
    virtual vec3dTransVolThreeD * create( void ) const;    
    virtual ~vec3dTransVolThreeD();
    virtual aFY Y( aFX const & xx) const;
    virtual bool closed( int const & dir ) const;
  private:
    ptrVectorHandling<vec3dTwoD> _v3dTwoD;
    dtVector3 _v3dTwoD_1_0_0;
    dtVector3 _v3dTwoD_1_0_1;
    dtVector3 _v3dTwoD_1_1_0;
    dtVector3 _v3dTwoD_1_1_1;
    dtVector3 _v3dTwoD_3_0_0;
    dtVector3 _v3dTwoD_3_0_1;
    dtVector3 _v3dTwoD_3_1_0;
    dtVector3 _v3dTwoD_3_1_1;    
  };
  typedef analyticFunctionCompound<vec3dTransVolThreeD> vec3dTransVolThreeDCompound;
}
#endif	/* VEC3DTRANSVOLTHREED_H */

