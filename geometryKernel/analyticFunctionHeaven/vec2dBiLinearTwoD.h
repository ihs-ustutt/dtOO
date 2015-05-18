#ifndef VEC2DBILINEARTWOD_H
#define	VEC2DBILINEARTWOD_H

#include "vec2dTwoD.h"
#include <vector>
#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>

namespace dtOO {
  class vec2dBiLinearTwoD : public vec2dTwoD {
  public:
    dt__class(vec2dBiLinearTwoD, analyticFunction);     
    vec2dBiLinearTwoD();
    vec2dBiLinearTwoD(vec2dBiLinearTwoD const & orig);
    vec2dBiLinearTwoD(
      dtPoint2 const & p0, dtPoint2 const & p1, 
      dtPoint2 const & p2, dtPoint2 const & p3
    );
    vec2dBiLinearTwoD(
      dtPoint2 const & p0, dtPoint2 const & p2
    );    
    vec2dBiLinearTwoD * clone( void ) const;
    vec2dBiLinearTwoD * create( void ) const;    
    virtual ~vec2dBiLinearTwoD();
    virtual aFY Y( aFX const & xx ) const;
    virtual bool closed( int const & dir ) const;    
  private:
    dtVector2 _p0;
    dtVector2 _p1;
    dtVector2 _p2;
    dtVector2 _p3;
  };
}
#endif	/* VEC2DBILINEARTWOD_H */