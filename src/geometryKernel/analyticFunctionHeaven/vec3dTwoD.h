#ifndef VEC3DTWOD_H
#define	VEC3DTWOD_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include "vec3dFunction.h"
#include <interfaceHeaven/vectorHandling.h>

namespace dtOO {
  class renderInterface;
  class dtTransformer;
  
  class vec3dTwoD : public vec3dFunction {
  public:
    dt__class(vec3dTwoD, analyticFunction);     
    vec3dTwoD();
    vec3dTwoD(const vec3dTwoD& orig);
    virtual ~vec3dTwoD();
    virtual vec3dTwoD * clone( void ) const = 0;
    virtual vec3dTwoD * cloneTransformed( 
      dtTransformer const * const dtT 
    ) const = 0;   
    virtual vec3dTwoD * create( void ) const = 0;    
    virtual aFY Y( aFX const & xx ) const = 0;
    virtual aFY Y( float const & x0, float const & x1 ) const;
    virtual int xDim( void ) const;
    void setMax(int const & dir, float const & max);
    void setMin(int const & dir, float const & min);
    virtual float xMin( int const & dir) const;
    virtual float xMax( int const & dir) const;   
    virtual std::vector<dtVector3> DYdtVector3( aFX const & xx ) const;
    std::vector<dtVector3> DYdtVector3( float const & x0, float const & x1 ) const;
    dtVector3 unitNdtVector3( aFX const & xx ) const;
    dtVector3 unitNdtVector3Percent( aFX const & xx ) const;
	  using analyticFunction::x_percent;
    using analyticFunction::percent_x;
    aFX x_percent(float const & x0, float const & x1) const;
    aFX percent_x(float const & x0, float const & x1) const;
    dtPoint3 YdtPoint3(aFX const & xx) const;
    dtPoint3 YdtPoint3(float const & x0, float const & x1) const;
    dtPoint3 YdtPoint3Percent(aFX const & xx) const;
    dtPoint3 YdtPoint3Percent(float const & x0, float const & x1) const;    
    vectorHandling< renderInterface * > getRender( void ) const;
    aFX operator%(const aFX &percent) const;
//    float length( aFX const & x0, aFX const & x1 ) const;
  private:
//	  float length( int const & nP, aFX const & x0, aFX const & x1 ) const;    
  private:    
    float _min[2];
    float _max[2];
  };
  dt__H_addCloneForpVH(vec3dTwoD);    
}
#endif	/* VEC3DTWOD_H */
