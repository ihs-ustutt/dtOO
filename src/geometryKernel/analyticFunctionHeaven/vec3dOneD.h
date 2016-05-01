#ifndef VEC3DONED_H
#define	VEC3DONED_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include "vec3dFunction.h"

namespace dtOO {
  class dtTransformer;
  
  class vec3dOneD : public vec3dFunction {
  public:
    dt__class(vec3dOneD, analyticFunction);     
    vec3dOneD();
    vec3dOneD(const vec3dOneD& orig);
    virtual ~vec3dOneD();
    virtual vec3dOneD * clone( void ) const = 0;
    virtual vec3dOneD * cloneTransformed( 
      dtTransformer const * const dtT 
    ) const = 0;      
    virtual vec3dOneD * create( void ) const = 0;    
    virtual aFY Y( float const & xx) const;
    virtual aFY Y(aFX const & xx) const = 0;
    virtual int xDim( void ) const;
    void setMin( float const & min );
    void setMax( float const & max );
    void setMax(int const & dir, float const & max);
    void setMin(int const & dir, float const & min);    
    virtual float xMin( int const & dir) const;
    virtual float xMax( int const & dir) const;   
    virtual dtVector3 DYdtVector3( float const & xx ) const;
    virtual dtVector3 DYdtVector3Percent(float const & xx) const;
	  float x_percent(float const & xx) const;
    float percent_x(float const & xx) const;
    dtPoint3 YdtPoint3(float const & xx) const;
    dtPoint3 YdtPoint3Percent(float const & xx) const;
    float length( float const & x1 ) const;
    float length( void ) const;
    float operator%( const float &percent ) const;     
  private:
	  float length( int const & nP, float const & x1 ) const;
  private:    
    float _min;
    float _max;
  };
  dt__H_addCloneForpVH(vec3dOneD);   
}
#endif	/* VEC3DONED_H */