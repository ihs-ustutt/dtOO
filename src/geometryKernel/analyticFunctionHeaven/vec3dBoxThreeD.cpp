#include "vec3dBoxThreeD.h"
#include "dtLinearAlgebra.h"
#include <logMe/logMe.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/staticPropertiesHandler.h>
#include "vec3dTwoD.h"

#include "analyticFunctionTransformed.h"

namespace dtOO {
	vec3dBoxThreeD::vec3dBoxThreeD() : vec3dThreeD() {
	}

	vec3dBoxThreeD::vec3dBoxThreeD(const vec3dBoxThreeD& orig) : vec3dThreeD(orig) {
		_p0 = orig._p0;
		_p1 = orig._p1;
		for (int ii=0; ii<3; ii++) {
			setMin( ii, orig.xMin(ii) );
			setMax( ii, orig.xMax(ii) );
		}
	}

	vec3dBoxThreeD::~vec3dBoxThreeD() {
	}
	
  vec3dBoxThreeD::vec3dBoxThreeD(dtPoint3 const & p0, dtPoint3 const & p1) : vec3dThreeD() {
		_p0 = p0;
		_p1 = p1;
		setMin( 0, 0. );
		setMin( 1, 0. );
		setMin( 2, 0. );
		setMax( 0, 1. );
		setMax( 1, 1. );				
		setMax( 2, 1. );
	}
	
	aFY vec3dBoxThreeD::Y( aFX const & xx) const {
		dtReal x0 = xx[0];
		dtReal x1 = xx[1];
		dtReal x2 = xx[2];
		
		
		dtPoint3 rV 
		= 
		_p0 
		+ dtLinearAlgebra::unitNormal0 * (_p1.x()-_p0.x())*x0
		+ dtLinearAlgebra::unitNormal1 * (_p1.y()-_p0.y())*x1
		+ dtLinearAlgebra::unitNormal2 * (_p1.z()-_p0.z())*x2;
		
//    dtVector3 rV =
//        (1.-x0) * dtLinearAlgebra::toDtVector3(_v3dTwoD[1].YdtPoint3Percent(x1, x2)) 
//			+ x0 * dtLinearAlgebra::toDtVector3(_v3dTwoD[3].YdtPoint3Percent(x1, x2))
//      + (1.-x1) * dtLinearAlgebra::toDtVector3(_v3dTwoD[4].YdtPoint3Percent(x0, x2)) 
//			+ x1 * dtLinearAlgebra::toDtVector3(_v3dTwoD[5].YdtPoint3Percent(x0, x2))
//      + (1.-x2) * dtLinearAlgebra::toDtVector3(_v3dTwoD[2].YdtPoint3Percent(x0, x1)) 
//			+ x2 * dtLinearAlgebra::toDtVector3(_v3dTwoD[0].YdtPoint3Percent(x0, x1))
//      - (1.-x0) * ( (1.-x1) * dtLinearAlgebra::toDtVector3(_v3dTwoD[1].YdtPoint3Percent(0., x2)) 
//			+ x1 * dtLinearAlgebra::toDtVector3(_v3dTwoD[1].YdtPoint3Percent(1., x2)) )
//      - x0      * ( (1.-x1) * dtLinearAlgebra::toDtVector3(_v3dTwoD[3].YdtPoint3Percent(0., x2)) 
//			+ x1 * dtLinearAlgebra::toDtVector3(_v3dTwoD[3].YdtPoint3Percent(1., x2)) )
//      - (1.-x1) * ( (1.-x2) * dtLinearAlgebra::toDtVector3(_v3dTwoD[4].YdtPoint3Percent(x0, 0.)) 
//			+ x2 * dtLinearAlgebra::toDtVector3(_v3dTwoD[4].YdtPoint3Percent(x0, 1.)) )
//      - x1      * ( (1.-x2) * dtLinearAlgebra::toDtVector3(_v3dTwoD[5].YdtPoint3Percent(x0, 0.)) 
//			+ x2 * dtLinearAlgebra::toDtVector3(_v3dTwoD[5].YdtPoint3Percent(x0, 1.)) )
//      - (1.-x2) * ( (1.-x0) * dtLinearAlgebra::toDtVector3(_v3dTwoD[2].YdtPoint3Percent(0., x1)) 
//			+ x0 * dtLinearAlgebra::toDtVector3(_v3dTwoD[2].YdtPoint3Percent(1., x1)) )
//      - x2      * ( (1.-x0) * dtLinearAlgebra::toDtVector3(_v3dTwoD[0].YdtPoint3Percent(0., x1)) 
//			+ x0 * dtLinearAlgebra::toDtVector3(_v3dTwoD[0].YdtPoint3Percent(1., x1)) )
//      + (1.-x0) * ( 
//          (1.-x1) * ( 
//            (1.-x2) * _v3dTwoD_1_0_0 + x2 * _v3dTwoD_1_0_1
//          )
//          +
//          x1 * ( 
//            (1.-x2) * _v3dTwoD_1_1_0 + x2 * _v3dTwoD_1_1_1
//          )            
//        )
//      + x0 * ( 
//          (1.-x1) * ( 
//            (1.-x2) * _v3dTwoD_3_0_0 + x2 * _v3dTwoD_3_0_1
//          )
//          +
//          x1 * ( 
//            (1.-x2) * _v3dTwoD_3_1_0 + x2 * _v3dTwoD_3_1_1
//          )            
//        );      
		
		aFY yy(3, 0.);
		yy[0] = rV.x();
		yy[1] = rV.y();
		yy[2] = rV.z();
		
		return yy;
	}

  vec3dBoxThreeD * vec3dBoxThreeD::clone( void ) const {
		return new vec3dBoxThreeD(*this);
	}

	vec3dBoxThreeD * vec3dBoxThreeD::cloneTransformed( 
    dtTransformer const * const dtT 
  ) const {
		return new analyticFunctionTransformed< vec3dBoxThreeD >(*this, dtT);
	}    
	
  vec3dBoxThreeD * vec3dBoxThreeD::create( void ) const {
		return new vec3dBoxThreeD();
	}
	
	/**
   * @todo Fix closed method. Currently return value is always false!
   */
  bool vec3dBoxThreeD::closed( dtInt const & dir ) const {
    return false;
	}
}
