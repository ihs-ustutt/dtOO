#include "rotatingMap2dTo3d.h"
#include <logMe/logMe.h>
#include "map2dTo3d.h"
#include "map1dTo3d.h"
#include "splineCurve3d.h"
#include <geometryEngine/dtCurve.h>

namespace dtOO {
	rotatingMap2dTo3d::rotatingMap2dTo3d() : map3dTo3d() {
	}

	rotatingMap2dTo3d::rotatingMap2dTo3d(const rotatingMap2dTo3d& orig) : map3dTo3d(orig) {
		_m2d.reset( orig._m2d->clone() );
		_vv = orig._vv;
	}

	rotatingMap2dTo3d::~rotatingMap2dTo3d() {
	}
	
  rotatingMap2dTo3d::rotatingMap2dTo3d( dtVector3 const & vv, map2dTo3d const * const m2d ) : map3dTo3d() {
		if (m2d->isClosedU() || m2d->isClosedV() ) {
			dt__THROW(rotatingMap2dTo3d(),
							<< "Surface is closed. This is not yet supported." << LOGDEL
							<< DTLOGEVAL(m2d->isClosedU()) << LOGDEL
							<< DTLOGEVAL(m2d->isClosedV()) );
		}
		_vv = dtLinearAlgebra::normalize(vv);
		_m2d.reset( m2d->clone() );
	}
  
	rotatingMap2dTo3d * rotatingMap2dTo3d::clone( void ) const {
		return new rotatingMap2dTo3d( *this );
	}
    
	rotatingMap2dTo3d * rotatingMap2dTo3d::create( void ) const {
		return new rotatingMap2dTo3d();
	}
	
  bool rotatingMap2dTo3d::isClosed( int const & dir) const {
    switch (dir) {
      case 0:
        return true;
        break;
      case 1:
        return false;
        break;
      case 2:
        return false;
        break;        
      default:
        dt__THROW(isClosed(),
              << DTLOGEVAL(dir) << LOGDEL
              << "dir should be 0, 1 or 2.");
    }
	}
  
	float rotatingMap2dTo3d::getMin( int const & dir) const {
    switch (dir) {
      case 0:
        return 0.;
        break;
      case 1:
        return _m2d->getUMin();
        break;
      case 2:
        return _m2d->getVMin();
        break;        
      default:
        dt__THROW(getMin(),
              << DTLOGEVAL(dir) << LOGDEL
              << "dir should be 0, 1 or 2.");
    }    		
	}
  
	float rotatingMap2dTo3d::getMax( int const & dir) const {
    switch (dir) {
      case 0:
        return 1.;
        break;
      case 1:
        return _m2d->getUMax();
        break;
      case 2:
        return _m2d->getVMax();
        break;        
      default:
        dt__THROW(getMax(),
              << DTLOGEVAL(dir) << LOGDEL
              << "dir should be 0, 1 or 2.");
    }    				
	}
    
	dtPoint3 rotatingMap2dTo3d::getPoint( float const & uu, float const & vv, float const & ww ) const {
		dtAffTransformation3 rot = dtLinearAlgebra::getRotation(_vv, uu*2*M_PI);
		return rot.transform( _m2d->getPoint(vv, ww) );
	}	
	
	float rotatingMap2dTo3d::u_phi(float const & arg) const {
    return arg / getMax(0);
	}

	float rotatingMap2dTo3d::u_phirvw(float const & phir, float const & vv, float const & ww) const {
    //
		// get radius
		//
    dtVector3 vXYZ = dtLinearAlgebra::toDtVector3(_m2d->getPoint(vv, ww));
    dtVector3 pointOnRotAx = _vv * dtLinearAlgebra::dotProduct(_vv, vXYZ);
    dtVector3 rr = vXYZ - pointOnRotAx;
		
		
		return (phir/dtLinearAlgebra::length(rr)) / getMax(0);
	}	
	
//	float rotatingMap2dTo3d::phi_u(float const & arg) const {
//    if (arg >= 0.) {
//      return ( getMax(0) * arg);
//    }
//    else {
//      float uu = getMax(0) * (fabs(arg));
//      return (map3dTo3d::getUMax() - uu);
//    }		
//	}
	
	float rotatingMap2dTo3d::v_m(float const & arg) const {
		ptrHandling< map1dTo3d > m1d( _m2d->pickConstVPercent(0., 0., 1.) );
		dt__PTRASS(
		  splineCurve3d const * s3d, 
			splineCurve3d::ConstDownCast(m1d.get())
		);
		
		return s3d->ptrConstDtCurve()->u_lPercent(arg);
	}

	float rotatingMap2dTo3d::v_mw(float const & mm, float const & ww) const {
		ptrHandling< map1dTo3d > m1d( _m2d->pickConstVPercent( _m2d->percent_v(ww), 0., 1.) );
		dt__PTRASS(
		  splineCurve3d const * s3d, 
			splineCurve3d::ConstDownCast(m1d.get())
		);
		
		return s3d->ptrConstDtCurve()->u_lPercent(mm);
	}	
	
//	float rotatingMap2dTo3d::m_v(float const & arg) const {
//		ptrHandling< map1dTo3d > m1d( _m2d->pickConstUPercent(0., 0., 1.) );
//		dt__PTRASS(
//		  splineCurve3d const * s3d, 
//			splineCurve3d::ConstDownCast(m1d.get())
//		);
//		
//		return s3d->ptrConstDtCurve()->lPercent_uPercent(arg);		
//	}
	
	float rotatingMap2dTo3d::w_s(float const & arg) const {
		ptrHandling< map1dTo3d > m1d( _m2d->pickConstUPercent(0., 0., 1.) );
		dt__PTRASS(
		  splineCurve3d const * s3d, 
			splineCurve3d::ConstDownCast(m1d.get())
		);
		
		return s3d->ptrConstDtCurve()->u_lPercent(arg);		
	}
	
//	float rotatingMap2dTo3d::s_w(float const & arg) const {
//		ptrHandling< map1dTo3d > m1d( _m2d->pickConstVPercent(0., 0., 1.) );
//		dt__PTRASS(
//		  splineCurve3d const * s3d, 
//			splineCurve3d::ConstDownCast(m1d.get())
//		);
//		
//		return s3d->ptrConstDtCurve()->lPercent_uPercent(arg);				
//	}
}
