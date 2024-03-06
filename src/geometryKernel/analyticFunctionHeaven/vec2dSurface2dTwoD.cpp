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

#include "vec2dSurface2dTwoD.h"
#include "dtLinearAlgebra.h"

#include <logMe/logMe.h>
#include <logMe/dtMacros.h>
#include <geometryEngine/dtSurface2d.h>

#include "analyticFunctionTransformed.h"

namespace dtOO {
	vec2dSurface2dTwoD::vec2dSurface2dTwoD() : vec2dTwoD() {
	}

	vec2dSurface2dTwoD::vec2dSurface2dTwoD(const vec2dSurface2dTwoD& orig) : vec2dTwoD(orig) {
		_dtS2d.reset( orig._dtS2d->clone() );
		setMin( 0, _dtS2d->minU() );
		setMin( 1, _dtS2d->minV() );
		setMax( 0, _dtS2d->maxU() );
		setMax( 1, _dtS2d->maxV() );		
	}

	vec2dSurface2dTwoD::~vec2dSurface2dTwoD() {
	}
	
	vec2dSurface2dTwoD::vec2dSurface2dTwoD(dtSurface2d const * const dtS2d) : vec2dTwoD() {
		_dtS2d.reset( dtS2d->clone() );
		setMin( 0, _dtS2d->minU() );
		setMin( 1, _dtS2d->minV() );
		setMax( 0, _dtS2d->maxU() );
		setMax( 1, _dtS2d->maxV() );
	}
	
	aFY vec2dSurface2dTwoD::Y( aFX const & xx) const {
		dtPoint2 p2d = _dtS2d->point(xx[0], xx[1]);
		
		aFY yy(2, 0.);
		
		yy[0] = p2d.x();
		yy[1] = p2d.y();
		
		return yy;
	}

  vec2dSurface2dTwoD * vec2dSurface2dTwoD::clone( void ) const {
		return new vec2dSurface2dTwoD(*this);
	}

	vec2dSurface2dTwoD * vec2dSurface2dTwoD::cloneTransformed( 
    dtTransformer const * const dtT 
  ) const {
		return new analyticFunctionTransformed< vec2dSurface2dTwoD >(*this, dtT);
	}  
	
  vec2dSurface2dTwoD * vec2dSurface2dTwoD::create( void ) const {
		return new vec2dSurface2dTwoD();
	}
	
	dtSurface2d const * vec2dSurface2dTwoD::ptrDtSurface2d( void ) const {
		return _dtS2d.get();
	}
	
  bool vec2dSurface2dTwoD::closed( dtInt const & dir ) const {
		return _dtS2d->closed(dir);
	}
}
