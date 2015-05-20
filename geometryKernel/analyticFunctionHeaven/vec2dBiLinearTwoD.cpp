#include "vec2dBiLinearTwoD.h"

#include <logMe/logMe.h>

namespace dtOO {
  vec2dBiLinearTwoD::vec2dBiLinearTwoD() : vec2dTwoD() {
  }

  vec2dBiLinearTwoD::vec2dBiLinearTwoD(vec2dBiLinearTwoD const & orig) : vec2dTwoD(orig) {
		_p0 = orig._p0;
		_p1 = orig._p1;
		_p2 = orig._p2;
		_p3 = orig._p3;
		for (int ii=0; ii<2; ii++) {
			setMin( ii, orig.xMin(ii) );
			setMax( ii, orig.xMax(ii) );
		}		
  }

  /**
	 *   (v)
	 *    A
	 *    |
	 * 
 	 *    p3      p2
	 *    +--------+
	 *    |        |
	 *    |        |
	 *    |        |
	 *    |        |
	 *    +--------+ --> (u)
	 *    p0      p1
   * 
   * according to: http://en.wikipedia.org/wiki/Bilinear_interpolation
   * 
   */  
  vec2dBiLinearTwoD::vec2dBiLinearTwoD(
      dtPoint2 const & p0, dtPoint2 const & p1, 
      dtPoint2 const & p2, dtPoint2 const & p3
	) : vec2dTwoD() {
		_p0 = dtLinearAlgebra::toDtVector2(p0);
		_p1 = dtLinearAlgebra::toDtVector2(p1);
		_p2 = dtLinearAlgebra::toDtVector2(p2);
		_p3 = dtLinearAlgebra::toDtVector2(p3);
		for (int ii=0; ii<2; ii++) {
			setMin( ii, 0. );
			setMax( ii, 1. );
		}				
  }

  vec2dBiLinearTwoD::vec2dBiLinearTwoD(
      dtPoint2 const & p0, dtPoint2 const & p2
	) : vec2dBiLinearTwoD(
      p0, dtPoint2(p2.x(), p0.y()), p2, dtPoint2(p0.x(), p2.y())
    ) {
  }
  
  vec2dBiLinearTwoD::~vec2dBiLinearTwoD() {
  }

  aFY vec2dBiLinearTwoD::Y( aFX const & xx ) const {
    dt__throwIf(xx.size()!=2, Y());

		float uu = xx[0];
		float vv = xx[1];
		
		dtVector2 vY(
		  _p0 * (1.-uu)*(1.-vv) + _p1 * uu*(1.-vv) + _p3 * (1.-uu)*vv + _p2 * uu*vv
		);
		
		aFY yy(2,0.);
		yy[0] = vY.x();
		yy[1] = vY.y();
    return yy;		
	}
	
	bool vec2dBiLinearTwoD::closed( int const & dir ) const {
		dt__throwIf( (dir!=0) && (dir!=1), closed );
		return false;
	}
	
  vec2dBiLinearTwoD * vec2dBiLinearTwoD::clone( void ) const {
    return new vec2dBiLinearTwoD( *this );
  }
  
  vec2dBiLinearTwoD * vec2dBiLinearTwoD::create( void ) const {
    return new vec2dBiLinearTwoD();
  }

  aFX vec2dBiLinearTwoD::invY( aFY const & yy ) const {
    dt__throwIf(yy.size()!=2, invY());

    float u0;
    float v0;
    float u1;
    float v1;
    
    int num_st 
    = 
    inverseBilerp( 
      _p0.x(), _p0.y(), _p1.x(), _p1.y(), 
      _p3.x(), _p3.y(), _p2.x(), _p2.y(), 
      yy[0], yy[1], 
      &u0, &v0, &u1, &v1 
    );
    
    dt__throwIf(num_st==0, invY() );
    
    aFX res = analyticFunction::aFXTwoD(-1., -1.);
    if (num_st == 1) {
      res[0] = std::min<float>(std::max<float>(u0, 0.), 1.);
      res[1] = std::min<float>(std::max<float>(v0, 0.), 1.);
    }
   	else if ( num_st == 2 ) {
      dt__throw(
        invY(),
        << dt__eval(u0) << std::endl
        << dt__eval(v0) << std::endl
        << dt__eval(u1) << std::endl
        << dt__eval(v1)
      );        
    }
    
    return res;
  }
  
  int vec2dBiLinearTwoD::equals( float a, float b, float tolerance ) {
      return ( a == b ) ||
        ( ( a <= ( b + tolerance ) ) &&
          ( a >= ( b - tolerance ) ) );
  }

  float vec2dBiLinearTwoD::cross2( 
    float const & x0, float const & y0, float const & x1, float const & y1 
  ) {
    return x0*y1 - y0*x1;
  }


  int vec2dBiLinearTwoD::in_range( 
    float val, float range_min, float range_max, float tol 
  ) {
    return ((val+tol) >= range_min) && ((val-tol) <= range_max);
  }

  /* 
   * Returns number of solutions found.  If there is one valid solution, it 
   * will be put in s and t.
   * Solution according to 
   * http://stackoverflow.com/questions/808441/inverse-bilinear-interpolation
   * 
   */
  int vec2dBiLinearTwoD::inverseBilerp( 
    float x0, float y0, float x1, float y1, 
    float x2, float y2, float x3, float y3, 
    float x, float y, 
    float* sout, float* tout, float* s2out, float* t2out 
  ) {
    int t_valid, t2_valid;

    float a  = cross2( x0-x, y0-y, x0-x2, y0-y2 );
    float b1 = cross2( x0-x, y0-y, x1-x3, y1-y3 );
    float b2 = cross2( x1-x, y1-y, x0-x2, y0-y2 );
    float c  = cross2( x1-x, y1-y, x1-x3, y1-y3 );
    float b  = 0.5 * (b1 + b2);

    float s, s2, t, t2;

    float am2bpc = a-2*b+c;
    /* this is how many valid s values we have */
    int num_valid_s = 0;

    if ( equals( am2bpc, 0, 1e-10 ) )
    {
      if ( equals( a-c, 0, 1e-10 ) )
      {
        /* Looks like the input is a line */
        /* You could set s=0.5 and solve for t if you wanted to */
        return 0;
      }
      s = a / (a-c);
      if ( in_range( s, 0, 1, 1e-10 ) )
        num_valid_s = 1;
    }
    else
    {
      float sqrtbsqmac = sqrt( b*b - a*c );
      s  = ((a-b) - sqrtbsqmac) / am2bpc;
      s2 = ((a-b) + sqrtbsqmac) / am2bpc;
      num_valid_s = 0;
      if ( in_range( s, 0, 1, 1e-10 ) )
      {
        num_valid_s++;
        if ( in_range( s2, 0, 1, 1e-10 ) )
          num_valid_s++;
      }
      else
      {
        if ( in_range( s2, 0, 1, 1e-10 ) )
        {
          num_valid_s++;
          s = s2;
        }
      }
    }

    if ( num_valid_s == 0 )
      return 0;

    t_valid = 0;
    if ( num_valid_s >= 1 )
    {
      float tdenom_x = (1-s)*(x0-x2) + s*(x1-x3);
      float tdenom_y = (1-s)*(y0-y2) + s*(y1-y3);
      t_valid = 1;
      if ( equals( tdenom_x, 0, 1e-10 ) && equals( tdenom_y, 0, 1e-10 ) )
      {
        t_valid = 0;
      }
      else
      {
        /* Choose the more robust denominator */
        if ( fabs( tdenom_x ) > fabs( tdenom_y ) )
        {
          t = ( (1-s)*(x0-x) + s*(x1-x) ) / ( tdenom_x );
        }
        else
        {
          t = ( (1-s)*(y0-y) + s*(y1-y) ) / ( tdenom_y );
        }
        if ( !in_range( t, 0, 1, 1e-10 ) )
          t_valid = 0;
      }
    }

    /* Same thing for s2 and t2 */
    t2_valid = 0;
    if ( num_valid_s == 2 )
    {
      float tdenom_x = (1-s2)*(x0-x2) + s2*(x1-x3);
      float tdenom_y = (1-s2)*(y0-y2) + s2*(y1-y3);
      t2_valid = 1;
      if ( equals( tdenom_x, 0, 1e-10 ) && equals( tdenom_y, 0, 1e-10 ) )
      {
        t2_valid = 0;
      }
      else
      {
        /* Choose the more robust denominator */
        if ( fabs( tdenom_x ) > fabs( tdenom_y ) )
        {
          t2 = ( (1-s2)*(x0-x) + s2*(x1-x) ) / ( tdenom_x );
        }
        else
        {
          t2 = ( (1-s2)*(y0-y) + s2*(y1-y) ) / ( tdenom_y );
        }
        if ( !in_range( t2, 0, 1, 1e-10 ) )
          t2_valid = 0;
      }
    }

    /* Final cleanup */
    if ( t2_valid && !t_valid )
    {
      s = s2;
      t = t2;
      t_valid = t2_valid;
      t2_valid = 0;
    }

    /* Output */
    if ( t_valid )
    {
      *sout = s;
      *tout = t;
    }

    if ( t2_valid )
    {
      *s2out = s2;
      *t2out = t2;
    }

    return t_valid + t2_valid;
  }
}