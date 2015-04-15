#ifndef VEC2DTWOD_H
#define	VEC2DTWOD_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include "vec2dFunction.h"
#include <interfaceHeaven/vectorHandling.h>

namespace dtOO {
  class renderInterface;
  
  class vec2dTwoD : public vec2dFunction {
  public:
    dt__class(vec2dTwoD, analyticFunction);     
    vec2dTwoD();
    vec2dTwoD(const vec2dTwoD& orig);
    virtual ~vec2dTwoD();
    virtual vec2dTwoD * clone( void ) const = 0;
    virtual vec2dTwoD * create( void ) const = 0;    
    virtual aFY Y( aFX const & xx ) const = 0;
    virtual aFY Y( float const & x0, float const & x1 ) const;
    virtual int xDim( void ) const;
    void setMax(int const & dir, float const & max);
    void setMin(int const & dir, float const & min);
    virtual float xMin( int const & dir) const;
    virtual float xMax( int const & dir) const;   
    virtual std::vector<dtVector2> DYdtVector2( aFX const & xx ) const;
    std::vector<dtVector2> DYdtVector2( float const & x0, float const & x1 ) const;
	  aFX x_percent( aFX const & xx ) const;
    aFX percent_x( aFX const & xx ) const;
    aFX x_percent(float const & x0, float const & x1) const;
    aFX percent_x(float const & x0, float const & x1) const;
    dtPoint2 YdtPoint2(aFX const & xx) const;
    dtPoint2 YdtPoint2(float const & x0, float const & x1) const;
    dtPoint2 YdtPoint2Percent(aFX const & xx) const;
    dtPoint2 YdtPoint2Percent(float const & x0, float const & x1) const;    
    vectorHandling< renderInterface * > getRender( void ) const;
  private:    
    float _min[2];
    float _max[2];
  };
}
#endif	/* VEC2DTWOD_H */

