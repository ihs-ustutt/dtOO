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


  vec2dBiLinearTwoD::~vec2dBiLinearTwoD() {
  }

  //
	// according to: http://en.wikipedia.org/wiki/Bilinear_interpolation
	//
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
}