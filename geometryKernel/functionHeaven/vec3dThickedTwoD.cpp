#include "vec3dThickedTwoD.h"
#include <dtLinearAlgebra.h>

#include <logMe/logMe.h>
#include <progHelper.h>
#include "scaTwoD.h"

namespace dtOO {
	vec3dThickedTwoD::vec3dThickedTwoD() : vec3dTwoD() {
	}

	vec3dThickedTwoD::vec3dThickedTwoD(const vec3dThickedTwoD& orig) : vec3dTwoD(orig) {
		_v3d2d.reset( orig._v3d2d->clone() );
		_s2d.reset( orig._s2d->clone() );
		setMin( 0, _v3d2d->xMin(0) );
		setMin( 1, _v3d2d->xMin(1) );
		setMax( 0, _v3d2d->xMax(0) );
		setMax( 1, _v3d2d->xMax(1) );		
	}

	vec3dThickedTwoD::~vec3dThickedTwoD() {
	}
	
	vec3dThickedTwoD::vec3dThickedTwoD(
	  vec3dTwoD const * const v3d2d, scaTwoD const * const s2d
	) : vec3dTwoD() {
		_v3d2d.reset( v3d2d->clone() );
		_s2d.reset( s2d->clone() );
		setMin( 0, _v3d2d->xMin(0) );
		setMin( 1, _v3d2d->xMin(1) );
		setMax( 0, _v3d2d->xMax(0) );
		setMax( 1, _v3d2d->xMax(1) );
	}
	
	aFY vec3dThickedTwoD::Y( aFX const & xx) const {
		dtPoint3 yy = _v3d2d->YdtPoint3( xx );
		dtVector3 nn = _v3d2d->unitNdtVector3( xx );
		nn = dtVector3(nn.x(), nn.y(), 0.);
		dtLinearAlgebra::normalize(nn);

		aFX xxP = _v3d2d->percent_x(xx);
		float tt = _s2d->YFloatPercent( xxP[0], xxP[1] );

		dtPoint3 p3 = yy + tt * nn;

		aFY retY(3);
		
		retY[0] = p3.x();
		retY[1] = p3.y();
		retY[2] = p3.z();
		
		return retY;
	}

  vec3dThickedTwoD * vec3dThickedTwoD::clone( void ) const {
		return new vec3dThickedTwoD(*this);
	}
	
  vec3dThickedTwoD * vec3dThickedTwoD::create( void ) const {
		return new vec3dThickedTwoD();
	}
	
	/**
	 * @todo Is this correct? Is it never closed?
   */
  bool vec3dThickedTwoD::closed( int const & dir ) const {
		return false;
	}
}
