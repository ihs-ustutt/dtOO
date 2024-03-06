/*---------------------------------------------------------------------------*\
  dtOO < design tool Object-Oriented >
    
    Copyright (C) 2024 A. Tismer.
-------------------------------------------------------------------------------
License
    This file is part of dtOO.

    dtOO is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the LICENSE.txt file in the
    dtOO root directory for more details.

    You should have received a copy of the License along with dtOO.

\*---------------------------------------------------------------------------*/

#include "vec3dBiLinearTwoD.h"

#include <logMe/logMe.h>
#include <interfaceHeaven/staticPropertiesHandler.h>

#include "analyticFunctionTransformed.h"

namespace dtOO {
  vec3dBiLinearTwoD::vec3dBiLinearTwoD() : vec3dTwoD() {
  }

  vec3dBiLinearTwoD::vec3dBiLinearTwoD(vec3dBiLinearTwoD const & orig) 
    : vec3dTwoD(orig) {
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
  vec3dBiLinearTwoD::vec3dBiLinearTwoD(
      dtPoint3 const & p0, dtPoint3 const & p1, 
      dtPoint3 const & p2, dtPoint3 const & p3
	) : vec3dTwoD() {
    _p0 = p0;
    _p1 = p1;
    _p2 = p2;
    _p3 = p3;
		_v0 = dtLinearAlgebra::toDtVector3(_p0);
		_v1 = dtLinearAlgebra::toDtVector3(_p1);
		_v2 = dtLinearAlgebra::toDtVector3(_p2);
		_v3 = dtLinearAlgebra::toDtVector3(_p3);
		for (int ii=0; ii<2; ii++) {
			setMin( ii, 0. );
			setMax( ii, 1. );
		}				
  }

  vec3dBiLinearTwoD::~vec3dBiLinearTwoD() {
  }

  aFY vec3dBiLinearTwoD::Y( aFX const & xx ) const {
    dt__throwIf(xx.size()!=2, Y());

		dtReal uu = xx[0];
		dtReal vv = xx[1];
		
		dtVector3 vY(
		  _v0 * (1.-uu)*(1.-vv) + _v1 * uu*(1.-vv) + _v3 * (1.-uu)*vv + _v2 * uu*vv
		);
		
		aFY yy(3,0.);
		yy[0] = vY.x();
		yy[1] = vY.y();
    yy[2] = vY.z();
    return yy;		
	}
	
	bool vec3dBiLinearTwoD::closed( dtInt const & dir ) const {
		dt__throwIf( (dir!=0) && (dir!=1), closed );
		return false;
	}
	
  vec3dBiLinearTwoD * vec3dBiLinearTwoD::clone( void ) const {
    return new vec3dBiLinearTwoD( *this );
  }

	vec3dBiLinearTwoD * vec3dBiLinearTwoD::cloneTransformed( 
    dtTransformer const * const dtT 
  ) const {
		return new analyticFunctionTransformed< vec3dBiLinearTwoD >(*this, dtT);
	}     
  
  vec3dBiLinearTwoD * vec3dBiLinearTwoD::create( void ) const {
    return new vec3dBiLinearTwoD();
  }
}
