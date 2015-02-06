#include "rotatingMap2dTo3d.h"
#include <logMe/logMe.h>
#include "map2dTo3d.h"
#include "map1dTo3d.h"
#include "splineCurve3d.h"
#include "analyticSurface.h"
#include "rotatingSpline.h"
#include <geometryEngine/dtSurface.h>
#include <geometryEngine/geoBuilder/surfaceOfRevolution_curveRotateConstructOCC.h>
#include <geometryEngine/dtCurve.h>
#include <geometryEngine/geoBuilder/geomSurface_surfaceRotateConstructOCC.h>

namespace dtOO {
	rotatingMap2dTo3d::rotatingMap2dTo3d() : map3dTo3d() {
	}

	rotatingMap2dTo3d::rotatingMap2dTo3d(const rotatingMap2dTo3d& orig) : map3dTo3d(orig) {
		_m2d.reset( orig._m2d->clone() );
		_vv = orig._vv;
		_pp = orig._pp;
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
		_pp = dtPoint3(0.,0.,0.);
		_vv = dtLinearAlgebra::normalize(vv);
		_m2d.reset( m2d->clone() );
		//
		// set origin to default
		//
		correctOrigin();
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
//    dt__WARN_IFWM(uu<getUMin(), getPoint(), << uu << " < " << getUMin()); 
//		dt__WARN_IFWM(vv<getVMin(), getPoint(), << vv << " < " << getVMin());
//		dt__WARN_IFWM(ww<getWMin(), getPoint(), << ww << " < " << getWMin());
//    dt__WARN_IFWM(uu>getUMax(), getPoint(), << uu << " > " << getUMax()); 
//		dt__WARN_IFWM(vv>getVMax(), getPoint(), << vv << " > " << getVMax());
//		dt__WARN_IFWM(ww>getWMax(), getPoint(), << ww << " > " << getWMax());
		
		dtAffTransformation3 rot = dtLinearAlgebra::getRotation(_vv, uu*2*M_PI);
		return rot.transform( _m2d->getPoint(vv, ww) );
	}	
	
	dtPoint3 rotatingMap2dTo3d::reparamInVolume(dtPoint3 const & ppXYZ) const {
		float const extU = .0;
		float const extV = .1;
		float const extW = .1;
//	  DTINFOWF(
//			reparamInVolume(), 
//			<< logMe::dtFormat("Extension of uvw-range by (%d, %d, %d)") % extU % extV % extW
//		);
		
		return map3dTo3d::reparamInVolume(ppXYZ, dtVector3(extU, extV, extW) );
	}
	
	map2dTo3d * rotatingMap2dTo3d::segmentConstU( float const & uu ) const {
		analyticSurface const * aS = analyticSurface::ConstDownCast(_m2d.get());
		if (aS) {
			dt__pH(dtSurface) dtS(
			  geomSurface_surfaceRotateConstructOCC(
		      aS->ptrDtSurface(), dtPoint3(0,0,0), _vv, uu*2.*M_PI
			  ).result()
			);
			return new analyticSurface(dtS.get());
		}
		else {
			return map3dTo3d::segmentConstU(uu);
		}
	}

	map2dTo3d * rotatingMap2dTo3d::segmentConstV( float const & vv ) const {
		analyticSurface const * aS = analyticSurface::ConstDownCast(_m2d.get());
		if (aS) {
			map1dTo3d * m1d = aS->segmentConstUPercent(percent_v(vv), 0., 1.);
			dt__PTRASS(splineCurve3d * s3d, splineCurve3d::DownCast(m1d));
			dt__pH(dtSurface) dtS(
			  surfaceOfRevolution_curveRotateConstructOCC(
					*(s3d->ptrConstDtCurve()), _pp, _vv
			  ).result()
			);
			
			return new rotatingSpline(*dtS, _pp, _vv);
		}
		else {
			return map3dTo3d::segmentConstV(vv);
		}
	}
  
	map2dTo3d * rotatingMap2dTo3d::segmentConstW( float const & ww ) const {
		analyticSurface const * aS = analyticSurface::ConstDownCast(_m2d.get());
		if (aS) {
			map1dTo3d * m1d = aS->segmentConstVPercent(percent_w(ww), 0., 1.);
			dt__PTRASS(splineCurve3d * s3d, splineCurve3d::DownCast(m1d));
			dt__pH(dtSurface) dtS(
			  surfaceOfRevolution_curveRotateConstructOCC(
					*(s3d->ptrConstDtCurve()), _pp, _vv
			  ).result()
			);
			
			return new rotatingSpline(*dtS, _pp, _vv);
		}
		else {
			return map3dTo3d::segmentConstW(ww);
		}
	}	
	
	float rotatingMap2dTo3d::u_phi(float const & arg) const {
    return arg / (2.*M_PI);
	}

	float rotatingMap2dTo3d::u_phirvw(float const & phir, float const & vv, float const & ww) const {
    //
		// get radius
		//
    dtVector3 vXYZ = dtLinearAlgebra::toDtVector3(_m2d->getPoint(vv, ww));
    dtVector3 pointOnRotAx = _vv * dtLinearAlgebra::dotProduct(_vv, vXYZ);
    dtVector3 rr = vXYZ - pointOnRotAx;
		
		
		return (phir/dtLinearAlgebra::length(rr)) / (2.*M_PI);//getMax(0);
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
		ptrHandling< map1dTo3d > m1d( _m2d->segmentConstVPercent(0., 0., 1.) );
		dt__PTRASS(
		  splineCurve3d const * s3d, 
			splineCurve3d::ConstDownCast(m1d.get())
		);
		
		return s3d->ptrConstDtCurve()->u_lPercent(arg);
	}

	float rotatingMap2dTo3d::v_mw(float const & mm, float const & ww) const {
		ptrHandling< map1dTo3d > m1d( _m2d->segmentConstVPercent( _m2d->percent_v(ww), 0., 1.) );
		dt__PTRASS(
		  splineCurve3d const * s3d, 
			splineCurve3d::ConstDownCast(m1d.get())
		);
		
		return s3d->ptrConstDtCurve()->u_l(mm);
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
		ptrHandling< map1dTo3d > m1d( _m2d->segmentConstUPercent(0., 0., 1.) );
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

	void rotatingMap2dTo3d::correctOrigin() {
		dtVector3 dist = _m2d->getPointPercent(0.,0.) - _pp;
		if ( (dist*_vv) != 0. ) {
			float adjusting 
		  = 
			dtLinearAlgebra::dotProduct(dist, _vv)/dtLinearAlgebra::length(_vv);

			_pp = _pp + _vv  * adjusting;

			DTWARNINGWF(correctOrigin(),
							<< DTLOGEVAL(dist*_vv) << LOGDEL 
							<< "Origin of rotSpline is not correct!" << LOGDEL
							<< "Move origin to " << DTLOGPOI3D(_pp) );        
		}
	}

  std::string rotatingMap2dTo3d::dumpToString( void ) const {
		std::stringstream ss;
		
		ss 
		<< DTLOGEVAL(_m2d->virtualClassName()) << LOGDEL
    << DTLOGPOI3D(_pp) << LOGDEL
		<< DTLOGVEC3D(_vv);
		
		return ss.str();		
	}
}
