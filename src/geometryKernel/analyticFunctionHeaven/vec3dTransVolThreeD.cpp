#include "vec3dTransVolThreeD.h"
#include "dtLinearAlgebra.h"
#include <logMe/logMe.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/staticPropertiesHandler.h>
#include "vec3dTwoD.h"

#include "analyticFunctionTransformed.h"

namespace dtOO {
	vec3dTransVolThreeD::vec3dTransVolThreeD() : vec3dThreeD() {
	}

	vec3dTransVolThreeD::vec3dTransVolThreeD(const vec3dTransVolThreeD& orig) : vec3dThreeD(orig) {
		for (int ii=0; ii<6; ii++) {
		  _v3dTwoD.push_back( orig._v3dTwoD[ii].clone() );
		}	
		for (int ii=0; ii<3; ii++) {
			setMin( ii, orig.xMin(ii) );
			setMax( ii, orig.xMax(ii) );
		}
    _v3dTwoD_1_0_0 = dtLinearAlgebra::toDtVector3(_v3dTwoD[1].YdtPoint3Percent(0.,0.));
    _v3dTwoD_1_0_1 = dtLinearAlgebra::toDtVector3(_v3dTwoD[1].YdtPoint3Percent(0.,1.));
    _v3dTwoD_1_1_0 = dtLinearAlgebra::toDtVector3(_v3dTwoD[1].YdtPoint3Percent(1.,0.));
    _v3dTwoD_1_1_1 = dtLinearAlgebra::toDtVector3(_v3dTwoD[1].YdtPoint3Percent(1.,1.));
    _v3dTwoD_3_0_0 = dtLinearAlgebra::toDtVector3(_v3dTwoD[3].YdtPoint3Percent(0.,0.));
    _v3dTwoD_3_0_1 = dtLinearAlgebra::toDtVector3(_v3dTwoD[3].YdtPoint3Percent(0.,1.));
    _v3dTwoD_3_1_0 = dtLinearAlgebra::toDtVector3(_v3dTwoD[3].YdtPoint3Percent(1.,0.));
    _v3dTwoD_3_1_1 = dtLinearAlgebra::toDtVector3(_v3dTwoD[3].YdtPoint3Percent(1.,1.));			
	}

	vec3dTransVolThreeD::~vec3dTransVolThreeD() {
	}
	
  vec3dTransVolThreeD::vec3dTransVolThreeD(
    vec3dTwoD const * const v3d2d0,  
    vec3dTwoD const * const v3d2d1,
    vec3dTwoD const * const v3d2d2,
    vec3dTwoD const * const v3d2d3,
    vec3dTwoD const * const v3d2d4,
    vec3dTwoD const * const v3d2d5
  ) : vec3dThreeD() {
		_v3dTwoD.push_back( v3d2d0->clone() );
		_v3dTwoD.push_back( v3d2d1->clone() );
		_v3dTwoD.push_back( v3d2d2->clone() );
		_v3dTwoD.push_back( v3d2d3->clone() );
		_v3dTwoD.push_back( v3d2d4->clone() );
		_v3dTwoD.push_back( v3d2d5->clone() );
		setMin( 0, 0. );
		setMin( 1, 0. );
		setMin( 2, 0. );
		setMax( 0, 1. );
		setMax( 1, 1. );				
		setMax( 2, 1. );
    _v3dTwoD_1_0_0 = dtLinearAlgebra::toDtVector3(_v3dTwoD[1].YdtPoint3Percent(0.,0.));
    _v3dTwoD_1_0_1 = dtLinearAlgebra::toDtVector3(_v3dTwoD[1].YdtPoint3Percent(0.,1.));
    _v3dTwoD_1_1_0 = dtLinearAlgebra::toDtVector3(_v3dTwoD[1].YdtPoint3Percent(1.,0.));
    _v3dTwoD_1_1_1 = dtLinearAlgebra::toDtVector3(_v3dTwoD[1].YdtPoint3Percent(1.,1.));
    _v3dTwoD_3_0_0 = dtLinearAlgebra::toDtVector3(_v3dTwoD[3].YdtPoint3Percent(0.,0.));
    _v3dTwoD_3_0_1 = dtLinearAlgebra::toDtVector3(_v3dTwoD[3].YdtPoint3Percent(0.,1.));
    _v3dTwoD_3_1_0 = dtLinearAlgebra::toDtVector3(_v3dTwoD[3].YdtPoint3Percent(1.,0.));
    _v3dTwoD_3_1_1 = dtLinearAlgebra::toDtVector3(_v3dTwoD[3].YdtPoint3Percent(1.,1.));		
	}
	
	aFY vec3dTransVolThreeD::Y( aFX const & xx) const {
		dtReal x0 = xx[0];
		dtReal x1 = xx[1];
		dtReal x2 = xx[2];
		
    dtVector3 rV =
        (1.-x0) * dtLinearAlgebra::toDtVector3(_v3dTwoD[1].YdtPoint3Percent(x1, x2)) 
			+ x0 * dtLinearAlgebra::toDtVector3(_v3dTwoD[3].YdtPoint3Percent(x1, x2))
      + (1.-x1) * dtLinearAlgebra::toDtVector3(_v3dTwoD[4].YdtPoint3Percent(x0, x2)) 
			+ x1 * dtLinearAlgebra::toDtVector3(_v3dTwoD[5].YdtPoint3Percent(x0, x2))
      + (1.-x2) * dtLinearAlgebra::toDtVector3(_v3dTwoD[2].YdtPoint3Percent(x0, x1)) 
			+ x2 * dtLinearAlgebra::toDtVector3(_v3dTwoD[0].YdtPoint3Percent(x0, x1))
      - (1.-x0) * ( (1.-x1) * dtLinearAlgebra::toDtVector3(_v3dTwoD[1].YdtPoint3Percent(0., x2)) 
			+ x1 * dtLinearAlgebra::toDtVector3(_v3dTwoD[1].YdtPoint3Percent(1., x2)) )
      - x0      * ( (1.-x1) * dtLinearAlgebra::toDtVector3(_v3dTwoD[3].YdtPoint3Percent(0., x2)) 
			+ x1 * dtLinearAlgebra::toDtVector3(_v3dTwoD[3].YdtPoint3Percent(1., x2)) )
      - (1.-x1) * ( (1.-x2) * dtLinearAlgebra::toDtVector3(_v3dTwoD[4].YdtPoint3Percent(x0, 0.)) 
			+ x2 * dtLinearAlgebra::toDtVector3(_v3dTwoD[4].YdtPoint3Percent(x0, 1.)) )
      - x1      * ( (1.-x2) * dtLinearAlgebra::toDtVector3(_v3dTwoD[5].YdtPoint3Percent(x0, 0.)) 
			+ x2 * dtLinearAlgebra::toDtVector3(_v3dTwoD[5].YdtPoint3Percent(x0, 1.)) )
      - (1.-x2) * ( (1.-x0) * dtLinearAlgebra::toDtVector3(_v3dTwoD[2].YdtPoint3Percent(0., x1)) 
			+ x0 * dtLinearAlgebra::toDtVector3(_v3dTwoD[2].YdtPoint3Percent(1., x1)) )
      - x2      * ( (1.-x0) * dtLinearAlgebra::toDtVector3(_v3dTwoD[0].YdtPoint3Percent(0., x1)) 
			+ x0 * dtLinearAlgebra::toDtVector3(_v3dTwoD[0].YdtPoint3Percent(1., x1)) )
      + (1.-x0) * ( 
          (1.-x1) * ( 
            (1.-x2) * _v3dTwoD_1_0_0 + x2 * _v3dTwoD_1_0_1
          )
          +
          x1 * ( 
            (1.-x2) * _v3dTwoD_1_1_0 + x2 * _v3dTwoD_1_1_1
          )            
        )
      + x0 * ( 
          (1.-x1) * ( 
            (1.-x2) * _v3dTwoD_3_0_0 + x2 * _v3dTwoD_3_0_1
          )
          +
          x1 * ( 
            (1.-x2) * _v3dTwoD_3_1_0 + x2 * _v3dTwoD_3_1_1
          )            
        );      
		
		aFY yy(3, 0.);
		yy[0] = rV.x();
		yy[1] = rV.y();
		yy[2] = rV.z();
		
		return yy;
	}

  vec3dTransVolThreeD * vec3dTransVolThreeD::clone( void ) const {
		return new vec3dTransVolThreeD(*this);
	}

	vec3dTransVolThreeD * vec3dTransVolThreeD::cloneTransformed( 
    dtTransformer const * const dtT 
  ) const {
		return new analyticFunctionTransformed< vec3dTransVolThreeD >(*this, dtT);
	}    
	
  vec3dTransVolThreeD * vec3dTransVolThreeD::create( void ) const {
		return new vec3dTransVolThreeD();
	}
	
	/**
   * @todo Fix closed method. Currently return value is always false!
   */
  bool vec3dTransVolThreeD::closed( int const & dir ) const {
    return false;
	}
}
