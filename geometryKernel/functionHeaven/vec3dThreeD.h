#ifndef VEC3DTHREED_H
#define	VEC3DTHREED_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include "vec3dFunction.h"
#include <interfaceHeaven/vectorHandling.h>

namespace dtOO {
  class renderInterface;
  
  class vec3dThreeD : public vec3dFunction {
  public:
    dt__CLASSSTD(vec3dThreeD, analyticFunction);     
    vec3dThreeD();
    vec3dThreeD(const vec3dThreeD& orig);
    virtual ~vec3dThreeD();
    virtual vec3dThreeD * clone( void ) const = 0;
    virtual vec3dThreeD * create( void ) const = 0;    
    virtual aFY Y( aFX const & xx ) const = 0;
    virtual aFY Y( float const & x0, float const & x1, float const & x2 ) const;
    virtual int xDim( void ) const;
    void setMax(int const & dir, float const & max);
    void setMin(int const & dir, float const & min);
    virtual float xMin( int const & dir) const;
    virtual float xMax( int const & dir) const;   
    virtual std::vector<dtVector3> DYdtVector3( aFX const & xx ) const;
	  aFX x_percent( aFX const & xx ) const;
    aFX percent_x( aFX const & xx ) const;
    dtPoint3 YdtPoint3(aFX const & xx) const;
    dtPoint3 YdtPoint3(float const & x0, float const & x1, float const & x2) const;
    dtPoint3 YdtPoint3Percent(aFX const & xx) const;
    dtPoint3 YdtPoint3Percent(float const & x0, float const & x1, float const & x2) const;    
    virtual vectorHandling< renderInterface * > getRender( void ) const;    
  private:    
    float _min[3];
    float _max[3];
  };
}
#endif	/* VEC3DTHREED_H */

