#ifndef VEC2DONED_H
#define	VEC2DONED_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include "vec2dFunction.h"

namespace dtOO {
  class vec2dOneD : public vec2dFunction {
  public:
    dt__class(vec2dOneD, analyticFunction);     
    vec2dOneD();
    vec2dOneD(const vec2dOneD& orig);
    virtual ~vec2dOneD();
    virtual vec2dOneD * clone( void ) const = 0;
    virtual vec2dOneD * create( void ) const = 0;     
    virtual aFY Y( float const & xx) const = 0;
    virtual aFY Y(aFX const & xx) const;
    virtual int xDim( void ) const;
    void setMin( float const & min );
    void setMax( float const & max );
    virtual float xMin( int const & dir) const;
    virtual float xMax( int const & dir) const;   
    virtual dtVector2 DYdtVector2( float const & xx ) const;
    dtVector2 unitNdtVector2( float const & xx ) const;
    dtVector2 unitNdtVector2Percent( float const & xx ) const;
	  float x_percent(float const & xx) const;
    float percent_x(float const & xx) const;
    dtPoint2 YdtPoint2(float const & xx) const;
    dtPoint2 YdtPoint2Percent(float const & xx) const;
    float length( float const & x1 ) const;
    float length( void ) const;
  private:
	  float length( int const & nP, float const & x1 ) const;
  private:    
    float _min;
    float _max;
  };
}
#endif	/* VEC2DFUNCTION_H */

