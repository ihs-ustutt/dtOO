#include "rotatingSpline.h"

#include <logMe/logMe.h>
#include <interfaceHeaven/staticPropertiesHandler.h>
#include "splineCurve3d.h"
#include <progHelper.h>
#include <geometryEngine/dtSurface.h>
#include <geometryEngine/dtCurve.h>
#include <interfaceHeaven/ptrHandling.h>

namespace dtOO {
  rotatingSpline::rotatingSpline() : analyticSurface() {
  }

  rotatingSpline::rotatingSpline( rotatingSpline const & orig ) : analyticSurface(orig) {
    _pp = orig._pp;
    _vv = orig._vv;
    _angle = orig._angle;
  }

  rotatingSpline::rotatingSpline(
    dtSurface const & dtS, 
    dtPoint3 const & pp,
    dtVector3 const & vv,
    float const & angle) : analyticSurface(&dtS) {
    
	  _vv = dtVector3(vv);
		_pp = dtPoint3(pp);
		_angle = angle;		
  }

  rotatingSpline::rotatingSpline(
    dtSurface const & dtS, 
    dtVector3 const & vv,
    float const & angle) : analyticSurface(&dtS) {
    
	  _vv = dtVector3(vv);
		_pp = dtPoint3(0,0,0);
		_angle = angle;		
		
		correctOrigin();
  }
  
  dtPoint3 rotatingSpline::getOrigin( void ) const {
    return _pp;
  }
  
  dtVector3 rotatingSpline::getRotVector( void ) const {
    return _vv;
  }

  float rotatingSpline::getAngle( void ) const {
    return _angle;
  }  
      
  float rotatingSpline::u_phi(float const & arg) const {
    return 
        map2dTo3d::getUMin() 
      + arg * (
          map2dTo3d::getUMax()-map2dTo3d::getUMin()
        ) 
        / 
        (2. * M_PI);
  }
  
  float rotatingSpline::phi_u(float const & arg) const {
    if (arg >= 0.) {
      return ( 2. * M_PI * (arg - map2dTo3d::getUMin()) / (map2dTo3d::getUMax()-map2dTo3d::getUMin()) );    
    }
    else {
      float uu = 2. * M_PI * (fabs(arg) - map2dTo3d::getUMin()) / (map2dTo3d::getUMax()-map2dTo3d::getUMin());
      return (map2dTo3d::getUMax() - uu);
    }
  }
  
  float rotatingSpline::v_m(float const & arg) const {
//    ptrHandling<dtCurve const> rr( getRadiusCurve() );
    return ptrConstRadiusCurve()->u_l(arg);
  }
  
  float rotatingSpline::m_v(float const & arg) const {
//    ptrHandling<dtCurve const> rr( getRadiusCurve() );
    return ptrConstRadiusCurve()->l_u(arg);    
  }
  
  dtPoint3 rotatingSpline::xyz_phiZ(float const & arg0, float const & arg1) const {
    return getPoint( u_phi(arg0), v_z(arg1) );
  }
  
  dtPoint2 rotatingSpline::uv_phiZ(float const & arg0, float const & arg1) const {
    return dtPoint2(u_phi(arg0), v_z(arg1));
  }
  
  dtPoint2 rotatingSpline::uv_deltaPhiRadiusDeltaM(float const & bU, float const & bV, float const & arg0, float const & arg1) const {
    float phi = phi_u(bU);
    float rr = r_v(bV);
    float mm = m_v(bV);
    
    return uv_phiRadiusM(phi*rr+arg0, mm+arg1);  
  }
  
  dtPoint2 rotatingSpline::uv_phiRadiusM(float const & arg0, float const & arg1) const {
    float const vv = v_m(arg1);
    float const rr = r_v(vv);
    float const phi = arg0 / rr;
    return dtPoint2( u_phi(phi), vv );
  }
  
  dtPoint2 rotatingSpline::uv_phiM(float const & arg0, float const & arg1) const {
    return dtPoint2( u_phi(arg0), v_m(arg1) );
  }
  
  float rotatingSpline::zMin( void ) const {
    return z_v( getMin(1) );
  }
  
  float rotatingSpline::zMax( void ) const {
    return z_v( getMax(1) );
  }
  
  float rotatingSpline::z_v(float const & arg) const {
    //
    //get point in x,y,z coordinates
    //
    dtPoint3 pointXYZ = getPoint(getMin(0), arg);
    //
    //calculate z-coordinate
    //
    return _vv * (pointXYZ - _pp);
  }

  float rotatingSpline::v_z(float const & arg) const {
    float minZ = zMin();
    float maxZ = zMax();
    
    //
    // iterate
    //
    float vv;
    float relativeTol 
		= 
		staticPropertiesHandler::getInstance()->getOptionFloat("xyz_resolution"); 
    std::vector<float> cR(2);
    std::vector<float> aR(2);
    cR[0] = map2dTo3d::getUMin();
    cR[1] = map2dTo3d::getUMax();
    aR[0] = map2dTo3d::getVMin();
    aR[1] = map2dTo3d::getVMax(); 
    
    vv = aR[0] + .5 * (aR[1]-aR[0]);
    for (int ii=0; ii<100; ii++) {
      float res = fabs(arg-z_v(vv));
      if( res < relativeTol ) {
        break;
      }
      if ( z_v(vv) < arg ) {
        aR[0] = vv;
      }
      else {
        aR[1] = vv;
      }
      vv = aR[0] + .5 * (aR[1]-aR[0]);
    }

    return vv;    
  }
  
  float rotatingSpline::r_v(float const & arg0) const {
    //
    //get point in x,y,z coordinates
    //
    dtPoint3 pXYZ = getPoint(getMin(0), arg0);
    dtPoint3 pointOnRotAx = _pp + _vv * z_v(arg0);
    dtVector3 distance = pXYZ - pointOnRotAx;
    
    //
    //calculate radius
    //
    return sqrt(distance.squared_length());    
  }
  
  float rotatingSpline::r_m(float const & arg0) const {
    float const vv = v_m(arg0);
    return r_v(vv);
  }

  rotatingSpline * rotatingSpline::clone( void ) const {
    return new rotatingSpline( *this );
  }

  rotatingSpline * rotatingSpline::create( void ) const {
    return new rotatingSpline();
  }

  rotatingSpline::~rotatingSpline() {
  }
  
  dtCurve const * rotatingSpline::ptrConstRadiusCurve( void ) const {
    map1dTo3d * m1d = pickConstUPercent(0., 0., 1.);
		dt__PTRASS(
		  splineCurve3d const * sC3,
		  splineCurve3d::ConstDownCast(m1d)
		);
		return sC3->ptrConstDtCurve();
  }
  
//  dtCurve * rotatingSpline::getCircleCurve( float const & vv ) const {
//    map1dTo3d * m1d = pickConstVPercent(0., 0., 1.);
//		dt__PTRASS(
//		  splineCurve3d const * sC3,
//		  splineCurve3d::ConstDownCast(m1d)
//		);
//		return sC3->ptrConstDtCurve()->clone();
//    
//  }
	
	void rotatingSpline::correctOrigin() {
		dtPoint3 start = map2dTo3d::getPointPercent(0.,0.);
		dtVector3 dist = start - _pp;
		if ( (dist*_vv) != 0. ) {
			float adjusting = (dist*_vv)/sqrt(_vv.squared_length());

			_pp = _pp + _vv  * adjusting;

			DTWARNINGWF(correctOrigin(),
							<< DTLOGEVAL(dist*_vv) << LOGDEL 
							<< "Origin of rotSpline is not correct!" << LOGDEL
							<< "Move origin to " << DTLOGPOI3D(_pp) );        
		}
	}
	
  map2dTo3d * rotatingSpline::pickPercent(
      float const & uu0, float const & uu1, 
      float const & vv0, float const & vv1
  ) const {
		ptrHandling<analyticSurface> aS( 
			analyticSurface::SecureCast(
				analyticSurface::pickPercent(uu0, uu1, vv0, vv1)
			) 
		);
		return new rotatingSpline( *(aS.get()->ptrDtSurface()), _pp, _vv, _angle);
  }	
}