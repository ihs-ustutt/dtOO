#include "vec3dTriLinearThreeD.h"

#include <logMe/logMe.h>
#include <interfaceHeaven/staticPropertiesHandler.h>

#include "analyticFunctionTransformed.h"

namespace dtOO {
  vec3dTriLinearThreeD::vec3dTriLinearThreeD() : vec3dThreeD() {
  }

  vec3dTriLinearThreeD::vec3dTriLinearThreeD(
    vec3dTriLinearThreeD const & orig
  ) : vec3dThreeD(orig) {
    _v000 = orig._v000;
    _v100 = orig._v100;
    _v010 = orig._v010;
    _v110 = orig._v110;
    _v001 = orig._v001;
    _v101 = orig._v101;
    _v011 = orig._v011;
    _v111 = orig._v111;
    _p000 = orig._p000;
    _p100 = orig._p100;
    _p010 = orig._p010;
    _p110 = orig._p110;
    _p001 = orig._p001;
    _p101 = orig._p101;
    _p011 = orig._p011;
    _p111 = orig._p111;  
  }

  /**
   * according to: https://en.wikipedia.org/wiki/Trilinear_interpolation
   * 
   */  
  vec3dTriLinearThreeD::vec3dTriLinearThreeD(
      dtPoint3 const & p000, dtPoint3 const & p100,
      dtPoint3 const & p010, dtPoint3 const & p110,
      dtPoint3 const & p001, dtPoint3 const & p101,
      dtPoint3 const & p011, dtPoint3 const & p111
	) : vec3dThreeD() {
    _p000 = p000;
    _p100 = p100;
    _p010 = p010;
    _p110 = p110;
    _p001 = p001;
    _p101 = p101;
    _p011 = p011;
    _p111 = p111;  
    _v000 = dtLinearAlgebra::toDtVector3(p000);
    _v100 = dtLinearAlgebra::toDtVector3(p100);
    _v010 = dtLinearAlgebra::toDtVector3(p010);
    _v110 = dtLinearAlgebra::toDtVector3(p110);
    _v001 = dtLinearAlgebra::toDtVector3(p001);
    _v101 = dtLinearAlgebra::toDtVector3(p101);
    _v011 = dtLinearAlgebra::toDtVector3(p011);
    _v111 = dtLinearAlgebra::toDtVector3(p111);    
		for (int ii=0; ii<3; ii++) {
			setMin( ii, 0. );
			setMax( ii, 1. );
		}				
  }

  vec3dTriLinearThreeD::vec3dTriLinearThreeD(
      dtPoint3 const & p000, dtPoint3 const & p111
	) : vec3dTriLinearThreeD(
      p000, 
      dtPoint3(p111.x(), p000.y(), p000.z()), 
      dtPoint3(p000.x(), p111.y(), p000.z()), 
      dtPoint3(p111.x(), p111.y(), p000.z()), 
      dtPoint3(p000.x(), p000.y(), p111.z()), 
      dtPoint3(p111.x(), p000.y(), p111.z()), 
      dtPoint3(p000.x(), p111.y(), p111.z()), 
      p111     
    ) {
  }
  
  vec3dTriLinearThreeD::~vec3dTriLinearThreeD() {
  }

  aFY vec3dTriLinearThreeD::Y( aFX const & xx ) const {
    dt__throwIf(xx.size()!=3, Y());

		float uu = xx[0];
		float vv = xx[1];
    float ww = xx[2];
    
    dtVector3 c00(_v000 * (1.-uu) + _v100 * uu);
    dtVector3 c10(_v010 * (1.-uu) + _v110 * uu);
    dtVector3 c01(_v001 * (1.-uu) + _v101 * uu);
    dtVector3 c11(_v011 * (1.-uu) + _v111 * uu);
    dtVector3 c0(c00 * (1.-vv) + c10 * vv);
    dtVector3 c1(c01 * (1.-vv) + c11 * vv);
		dtVector3 cc(c0 * (1.-ww) + c1 * ww);
		
		aFY yy(3,0.);
		yy[0] = cc.x();
		yy[1] = cc.y();
    yy[2] = cc.z();
    
    return yy;		
	}
	
	bool vec3dTriLinearThreeD::closed( int const & dir ) const {
		dt__throwIf( (dir!=0) && (dir!=1) && (dir!=2), closed() );
		return false;
	}
	
  vec3dTriLinearThreeD * vec3dTriLinearThreeD::clone( void ) const {
    return new vec3dTriLinearThreeD( *this );
  }

	vec3dTriLinearThreeD * vec3dTriLinearThreeD::cloneTransformed( 
    dtTransformer const * const dtT 
  ) const {
		return new analyticFunctionTransformed< vec3dTriLinearThreeD >(*this, dtT);
	}    
  
  vec3dTriLinearThreeD * vec3dTriLinearThreeD::create( void ) const {
    return new vec3dTriLinearThreeD();
  }
}