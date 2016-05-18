#include "vec3dSurfaceTwoD.h"
#include "dtLinearAlgebra.h"

#include <logMe/logMe.h>
#include <logMe/dtMacros.h>
#include <geometryEngine/dtSurface.h>

#include "analyticFunctionTransformed.h"

namespace dtOO {
	vec3dSurfaceTwoD::vec3dSurfaceTwoD() : vec3dTwoD() {
	}

	vec3dSurfaceTwoD::vec3dSurfaceTwoD(const vec3dSurfaceTwoD& orig) : vec3dTwoD(orig) {
		_dtS.reset( orig._dtS->clone() );
		setMin( 0, _dtS->minU() );
		setMin( 1, _dtS->minV() );
		setMax( 0, _dtS->maxU() );
		setMax( 1, _dtS->maxV() );		
	}

	vec3dSurfaceTwoD::~vec3dSurfaceTwoD() {
	}
	
	vec3dSurfaceTwoD::vec3dSurfaceTwoD(dtSurface const * const dtS) : vec3dTwoD() {
		_dtS.reset( dtS->clone() );
		setMin( 0, _dtS->minU() );
		setMin( 1, _dtS->minV() );
		setMax( 0, _dtS->maxU() );
		setMax( 1, _dtS->maxV() );
	}
	
	aFY vec3dSurfaceTwoD::Y( aFX const & xx) const {
		dtPoint3 p3d = _dtS->point(xx[0], xx[1]);
		
		aFY yy(3);
		
		yy[0] = p3d.x();
		yy[1] = p3d.y();
		yy[2] = p3d.z();
		
		return yy;
	}

  vec3dSurfaceTwoD * vec3dSurfaceTwoD::clone( void ) const {
		return new vec3dSurfaceTwoD(*this);
	}
	
	vec3dSurfaceTwoD * vec3dSurfaceTwoD::cloneTransformed( 
    dtTransformer const * const dtT 
  ) const {
		return new analyticFunctionTransformed< vec3dSurfaceTwoD >(*this, dtT);
	}  
  
  vec3dSurfaceTwoD * vec3dSurfaceTwoD::create( void ) const {
		return new vec3dSurfaceTwoD();
	}
	
	dtSurface * vec3dSurfaceTwoD::ptrDtSurface( void ) const {
		return _dtS.get();
	}
  
	dtSurface const * vec3dSurfaceTwoD::constPtrDtSurface( void ) const {
		return _dtS.get();
	}  
	
  bool vec3dSurfaceTwoD::closed( int const & dir ) const {
		return _dtS->closed(dir);
	}
  
  aFX vec3dSurfaceTwoD::invY(aFY const & yy) const {
    return analyticFunction::aFXTwoD(
      _dtS->reparam( dtPoint3(yy[0], yy[1], yy[2]) )
    );
  }
}
