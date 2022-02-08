#include "vec2dBiLinearTwoD.h"

#include <logMe/logMe.h>
#include <interfaceHeaven/staticPropertiesHandler.h>

#include "analyticFunctionTransformed.h"

namespace dtOO {
  vec2dBiLinearTwoD::vec2dBiLinearTwoD() : vec2dTwoD() {
  }

  vec2dBiLinearTwoD::vec2dBiLinearTwoD(vec2dBiLinearTwoD const & orig) : vec2dTwoD(orig) {
		_p0 = orig._p0;
		_p1 = orig._p1;
		_p2 = orig._p2;
		_p3 = orig._p3;    
		_v0 = orig._v0;
		_v1 = orig._v1;
		_v2 = orig._v2;
		_v3 = orig._v3;    
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
    _p0 = p0;
    _p1 = p1;
    _p2 = p2;
    _p3 = p3;
		_v0 = dtLinearAlgebra::toDtVector2(_p0);
		_v1 = dtLinearAlgebra::toDtVector2(_p1);
		_v2 = dtLinearAlgebra::toDtVector2(_p2);
		_v3 = dtLinearAlgebra::toDtVector2(_p3);
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

		dtReal uu = xx[0];
		dtReal vv = xx[1];
		
		dtVector2 vY(
		  _v0 * (1.-uu)*(1.-vv) + _v1 * uu*(1.-vv) + _v3 * (1.-uu)*vv + _v2 * uu*vv
		);
		
		aFY yy(2,0.);
		yy[0] = vY.x();
		yy[1] = vY.y();
    return yy;		
	}
	
	bool vec2dBiLinearTwoD::closed( dtInt const & dir ) const {
		dt__throwIf( (dir!=0) && (dir!=1), closed );
		return false;
	}
	
  vec2dBiLinearTwoD * vec2dBiLinearTwoD::clone( void ) const {
    return new vec2dBiLinearTwoD( *this );
  }

	vec2dBiLinearTwoD * vec2dBiLinearTwoD::cloneTransformed( 
    dtTransformer const * const dtT 
  ) const {
		return new analyticFunctionTransformed< vec2dBiLinearTwoD >(*this, dtT);
	}     
  
  vec2dBiLinearTwoD * vec2dBiLinearTwoD::create( void ) const {
    return new vec2dBiLinearTwoD();
  }

  aFX vec2dBiLinearTwoD::invY( aFY const & yy ) const {
    dt__throwIf(yy.size()!=2, invY());

    dtReal u0;
    dtReal v0;
    dtReal u1;
    dtReal v1;
    
    dtInt num_st 
    = 
    inverseBilerp( 
      _v0.x(), _v0.y(), _v1.x(), _v1.y(), 
      _v3.x(), _v3.y(), _v2.x(), _v2.y(), 
      yy[0], yy[1], 
      &u0, &v0, &u1, &v1 
    );
    
    if (num_st == 1) {
      return analyticFunction::aFXTwoD(
        std::min<dtReal>(std::max<dtReal>(u0, 0.), 1.),
        std::min<dtReal>(std::max<dtReal>(v0, 0.), 1.)
      );
    }
    
    //
    // simple inversion fails
    //
    dt__warning(
      invY(),
      << dt__point2d(_p0) << std::endl
      << dt__point2d(_p1) << std::endl
      << dt__point2d(_p2) << std::endl
      << dt__point2d(_p3) << std::endl
      << dt__eval(yy) << std::endl
      << "Simple invert failed. Try with general analyticFunction::invY()"
    );    
    return analyticFunction::invY(yy);
  }
  
  bool vec2dBiLinearTwoD::insideY( aFY const & yy ) const {
    return dtLinearAlgebra::isInsideQuadrangle(
      dtPoint2(yy[0], yy[1]), _p0, _p1, _p2, _p3
    );
  }
  
  dtInt vec2dBiLinearTwoD::equals( dtReal a, dtReal b, dtReal tolerance ) {
      return ( a == b ) ||
        ( ( a <= ( b + tolerance ) ) &&
          ( a >= ( b - tolerance ) ) );
  }

  dtReal vec2dBiLinearTwoD::cross2( 
    dtReal const & x0, dtReal const & y0, dtReal const & x1, dtReal const & y1 
  ) {
    return x0*y1 - y0*x1;
  }


  dtInt vec2dBiLinearTwoD::in_range( 
    dtReal val, dtReal range_min, dtReal range_max, dtReal tol 
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
  dtInt vec2dBiLinearTwoD::inverseBilerp( 
    dtReal x0, dtReal y0, dtReal x1, dtReal y1, 
    dtReal x2, dtReal y2, dtReal x3, dtReal y3, 
    dtReal x, dtReal y, 
    dtReal* sout, dtReal* tout, dtReal* s2out, dtReal* t2out 
  ) {
		dtReal tol
		=
		staticPropertiesHandler::getInstance()->getOptionFloat(
      "invY_precision"
    );		
    
    dtInt t_valid, t2_valid;

    dtReal a  = cross2( x0-x, y0-y, x0-x2, y0-y2 );
    dtReal b1 = cross2( x0-x, y0-y, x1-x3, y1-y3 );
    dtReal b2 = cross2( x1-x, y1-y, x0-x2, y0-y2 );
    dtReal c  = cross2( x1-x, y1-y, x1-x3, y1-y3 );
    dtReal b  = 0.5 * (b1 + b2);

    dtReal s, s2, t, t2;

    dtReal am2bpc = a-2*b+c;
    /* this is how many valid s values we have */
    dtInt num_valid_s = 0;

    if ( equals( am2bpc, 0, tol ) ) {
      if ( equals( a-c, 0, tol ) ){
        /* Looks like the input is a line */
        /* You could set s=0.5 and solve for t if you wanted to */
        return 0;
      }
      s = a / (a-c);
      if ( in_range( s, 0, 1, tol ) ) num_valid_s = 1;
    }
    else {
      dtReal sqrtbsqmac = sqrt( b*b - a*c );
      s  = ((a-b) - sqrtbsqmac) / am2bpc;
      s2 = ((a-b) + sqrtbsqmac) / am2bpc;
      num_valid_s = 0;
      if ( in_range( s, 0, 1, tol ) ) {
        num_valid_s++;
        if ( in_range( s2, 0, 1, tol ) ) num_valid_s++;
      }
      else {
        if ( in_range( s2, 0, 1, tol ) ) {
          num_valid_s++;
          s = s2;
        }
      }
    }

    if ( num_valid_s == 0 ) return 0;

    t_valid = 0;
    if ( num_valid_s >= 1 ) {
      dtReal tdenom_x = (1-s)*(x0-x2) + s*(x1-x3);
      dtReal tdenom_y = (1-s)*(y0-y2) + s*(y1-y3);
      t_valid = 1;
      if ( equals( tdenom_x, 0, tol ) && equals( tdenom_y, 0, tol ) ) {
        t_valid = 0;
      }
      else {
        /* Choose the more robust denominator */
        if ( fabs( tdenom_x ) > fabs( tdenom_y ) ) {
          t = ( (1-s)*(x0-x) + s*(x1-x) ) / ( tdenom_x );
        }
        else {
          t = ( (1-s)*(y0-y) + s*(y1-y) ) / ( tdenom_y );
        }
        if ( !in_range( t, 0, 1, tol ) ) t_valid = 0;
      }
    }

    /* Same thing for s2 and t2 */
    t2_valid = 0;
    if ( num_valid_s == 2 ) {
      dtReal tdenom_x = (1-s2)*(x0-x2) + s2*(x1-x3);
      dtReal tdenom_y = (1-s2)*(y0-y2) + s2*(y1-y3);
      t2_valid = 1;
      if ( equals( tdenom_x, 0, tol ) && equals( tdenom_y, 0, tol ) ) {
        t2_valid = 0;
      }
      else {
        /* Choose the more robust denominator */
        if ( fabs( tdenom_x ) > fabs( tdenom_y ) ) {
          t2 = ( (1-s2)*(x0-x) + s2*(x1-x) ) / ( tdenom_x );
        }
        else {
          t2 = ( (1-s2)*(y0-y) + s2*(y1-y) ) / ( tdenom_y );
        }
        if ( !in_range( t2, 0, 1, tol ) ) t2_valid = 0;
      }
    }

    /* Final cleanup */
    if ( t2_valid && !t_valid ) {
      s = s2;
      t = t2;
      t_valid = t2_valid;
      t2_valid = 0;
    }

    /* Output */
    if ( t_valid ) {
      *sout = s;
      *tout = t;
    }

    if ( t2_valid ) {
      *s2out = s2;
      *t2out = t2;
    }

    return t_valid + t2_valid;
  }
}