#ifndef vec3dTriLinearThreeD_H
#define	vec3dTriLinearThreeD_H

#include "vec3dThreeD.h"
#include <vector>
#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>

namespace dtOO {
  class dtTransformer;
  
  class vec3dTriLinearThreeD : public vec3dThreeD {
  public:
    dt__class(vec3dTriLinearThreeD, analyticFunction);     
    vec3dTriLinearThreeD();
    vec3dTriLinearThreeD(vec3dTriLinearThreeD const & orig);
    vec3dTriLinearThreeD(
      dtPoint3 const & p000, dtPoint3 const & p100,
      dtPoint3 const & p010, dtPoint3 const & p110,
      dtPoint3 const & p001, dtPoint3 const & p101,
      dtPoint3 const & p011, dtPoint3 const & p111
    );
    vec3dTriLinearThreeD(
      dtPoint3 const & p000, dtPoint3 const & p100,
      dtPoint3 const & p010, dtPoint3 const & p110,
      dtPoint3 const & p001, dtPoint3 const & p101,
      dtPoint3 const & p011, dtPoint3 const & p111,
      dtPoint3 const & xMin, dtPoint3 const & xMax
    );    
    vec3dTriLinearThreeD(
      dtPoint3 const & p000, dtPoint3 const & p111
    );    
    vec3dTriLinearThreeD * clone( void ) const;
    virtual vec3dTriLinearThreeD * cloneTransformed(
      dtTransformer const * const dtT 
    ) const;     
    vec3dTriLinearThreeD * create( void ) const;    
    virtual ~vec3dTriLinearThreeD();
    virtual aFY Y( aFX const & xx ) const;
    virtual bool closed( int const & dir ) const;    
  private:
    dtVector3 _v000;
    dtVector3 _v100;
    dtVector3 _v010;
    dtVector3 _v110;
    dtVector3 _v001;
    dtVector3 _v101;
    dtVector3 _v011;
    dtVector3 _v111;
    dtPoint3 _p000;
    dtPoint3 _p100;
    dtPoint3 _p010;
    dtPoint3 _p110;
    dtPoint3 _p001;
    dtPoint3 _p101;
    dtPoint3 _p011;
    dtPoint3 _p111;
  };
}
#endif	/* vec3dTriLinearThreeD_H */