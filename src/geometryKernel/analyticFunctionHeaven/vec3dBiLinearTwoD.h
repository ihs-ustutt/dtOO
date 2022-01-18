#ifndef vec3dBiLinearTwoD_H
#define	vec3dBiLinearTwoD_H

#include <dtOOTypeDef.h>

#include "vec3dTwoD.h"
#include <vector>
#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>

namespace dtOO {
  class dtTransformer;
  
  class vec3dBiLinearTwoD : public vec3dTwoD {
  public:
    dt__class(vec3dBiLinearTwoD, analyticFunction);     
    vec3dBiLinearTwoD();
    vec3dBiLinearTwoD(vec3dBiLinearTwoD const & orig);
    vec3dBiLinearTwoD(
      dtPoint3 const & p0, dtPoint3 const & p1, 
      dtPoint3 const & p2, dtPoint3 const & p3
    );
    vec3dBiLinearTwoD * clone( void ) const;
    virtual vec3dBiLinearTwoD * cloneTransformed(
      dtTransformer const * const dtT 
    ) const;           
    vec3dBiLinearTwoD * create( void ) const;    
    virtual ~vec3dBiLinearTwoD();
    virtual aFY Y( aFX const & xx ) const;
    virtual bool closed( dtInt const & dir ) const;    
  private:
    dtVector3 _v0;
    dtVector3 _v1;
    dtVector3 _v2;
    dtVector3 _v3;
    dtPoint3 _p0;
    dtPoint3 _p1;
    dtPoint3 _p2;
    dtPoint3 _p3;    
  };
}
#endif	/* vec3dBiLinearTwoD_H */