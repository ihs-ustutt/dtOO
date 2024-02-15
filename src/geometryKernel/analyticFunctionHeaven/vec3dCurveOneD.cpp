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

#include "vec3dCurveOneD.h"
#include "dtLinearAlgebra.h"

#include <logMe/logMe.h>
#include <logMe/dtMacros.h>
#include <geometryEngine/dtCurve.h>
#include <solid2dLine.h>
#include <discrete2dPoints.h>
#include <interfaceHeaven/staticPropertiesHandler.h>

#include "analyticFunctionTransformed.h"

namespace dtOO {
	vec3dCurveOneD::vec3dCurveOneD() : vec3dOneD() {
	}

	vec3dCurveOneD::vec3dCurveOneD(const vec3dCurveOneD& orig) : vec3dOneD(orig) {
		_dtC.reset( orig._dtC->clone() );
	}

	vec3dCurveOneD::~vec3dCurveOneD() {
	}
	
	vec3dCurveOneD::vec3dCurveOneD(dtCurve const * const dtC) : vec3dOneD() {
		_dtC.reset( dtC->clone() );
		setMin( dtC->getUMin() );
		setMax( dtC->getUMax() );
	}
	
	aFY vec3dCurveOneD::Y( aFX const & xx ) const {
		dtPoint3 p2d = _dtC->point(xx[0]);
		
		aFY yy(3, 0.);
		
		yy[0] = p2d.x();
		yy[1] = p2d.y();
		yy[2] = p2d.z();
		
		return yy;
	}

  vec3dCurveOneD * vec3dCurveOneD::clone( void ) const {
		return new vec3dCurveOneD(*this);
	}
  
	vec3dCurveOneD * vec3dCurveOneD::cloneTransformed( 
    dtTransformer const * const dtT 
  ) const {
		return new analyticFunctionTransformed< vec3dCurveOneD >(*this, dtT);
	}      
	
  vec3dCurveOneD * vec3dCurveOneD::create( void ) const {
		return new vec3dCurveOneD();
	}
	
  dtCurve * vec3dCurveOneD::ptrDtCurve(void) const {
    return _dtC.get();
  }
  
  dtCurve const * const vec3dCurveOneD::ptrConstDtCurve(void) const {
    return _dtC.get();
  }
	
	bool vec3dCurveOneD::closed( dtInt const & dir ) const {
		return _dtC->closed();
	}
}
