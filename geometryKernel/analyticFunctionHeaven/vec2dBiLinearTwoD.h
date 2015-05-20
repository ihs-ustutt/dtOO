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
    aFX invY(aFY const & yy) const;
  private:
    static float cross2(
      float const & x0, float const & y0, float const & x1, float const & y1
    );
    static int inverseBilerp( 
      float x0, float y0, float x1, float y1, 
      float x2, float y2, float x3, float y3, 
      float x, float y, 
      float* sout, float* tout, float* s2out, float* t2out 
    ); 
    static int in_range( 
      float val, float range_min, float range_max, float tol 
    );    
  static int equals(float a, float b, float tolerance);   
  private:
    dtVector2 _p0;
    dtVector2 _p1;
    dtVector2 _p2;
    dtVector2 _p3;
  };
}
#endif	/* VEC2DBILINEARTWOD_H */