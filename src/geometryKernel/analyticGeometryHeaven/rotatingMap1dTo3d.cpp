#include "rotatingMap1dTo3d.h"

#include <logMe/logMe.h>
#include <interfaceHeaven/staticPropertiesHandler.h>
#include "splineCurve3d.h"
#include <logMe/dtMacros.h>
#include <geometryEngine/dtSurface.h>
#include <geometryEngine/dtCurve.h>
#include <interfaceHeaven/ptrHandling.h>

namespace dtOO {
  rotatingMap1dTo3d::rotatingMap1dTo3d() : analyticSurface() {
  }

  rotatingMap1dTo3d::rotatingMap1dTo3d( 
    rotatingMap1dTo3d const & orig 
  ) : analyticSurface(orig) {
    _pp = orig._pp;
    _vv = orig._vv;
    _angle = orig._angle;
  }

  rotatingMap1dTo3d::rotatingMap1dTo3d(
    dtSurface const & dtS, 
    dtPoint3 const & pp,
    dtVector3 const & vv,
    float const & angle) : analyticSurface(&dtS) {
    
	  _vv = dtVector3(vv);
		_pp = dtPoint3(pp);
		_angle = angle;		
  }

  rotatingMap1dTo3d::rotatingMap1dTo3d(
    dtSurface const & dtS, 
    dtVector3 const & vv,
    float const & angle) : analyticSurface(&dtS) {
    
	  _vv = dtVector3(vv);
		_pp = dtPoint3(0,0,0);
		_angle = angle;		
		
		correctOrigin();
  }
  
  dtPoint3 rotatingMap1dTo3d::getOrigin( void ) const {
    return _pp;
  }
  
  dtVector3 rotatingMap1dTo3d::getRotVector( void ) const {
    return _vv;
  }

  float rotatingMap1dTo3d::getAngle( void ) const {
    return _angle;
  }  
      
  float rotatingMap1dTo3d::u_phi(float const & arg) const {
    return 
        map2dTo3d::getUMin() 
      + arg * (
          map2dTo3d::getUMax()-map2dTo3d::getUMin()
        ) 
        / 
        (2. * M_PI);
  }
  
  float rotatingMap1dTo3d::phi_u(float const & arg) const {
    if (arg >= 0.) {
      return 
        2. * M_PI * (arg - map2dTo3d::getUMin()) 
        / 
        (map2dTo3d::getUMax()-map2dTo3d::getUMin());    
    }
    else {
      float uu 
      = 
      2. * M_PI * (fabs(arg) - map2dTo3d::getUMin()) 
      / 
      (map2dTo3d::getUMax()-map2dTo3d::getUMin());
      return (map2dTo3d::getUMax() - uu);
    }
  }
  
  float rotatingMap1dTo3d::v_m(float const & arg) const {
//    ptrHandling<dtCurve const> rr( getRadiusCurve() );
    return ptrConstRadiusCurve()->u_l(arg);
  }
  
  float rotatingMap1dTo3d::m_v(float const & arg) const {
//    ptrHandling<dtCurve const> rr( getRadiusCurve() );
    return ptrConstRadiusCurve()->l_u(arg);    
  }
  
  dtPoint3 rotatingMap1dTo3d::xyz_phiZ(float const & arg0, float const & arg1) const {
    return getPoint( u_phi(arg0), v_z(arg1) );
  }
  
  dtPoint2 rotatingMap1dTo3d::uv_phiZ(float const & arg0, float const & arg1) const {
    return dtPoint2(u_phi(arg0), v_z(arg1));
  }
  
  dtPoint2 rotatingMap1dTo3d::uv_deltaPhiRadiusDeltaM(
    float const & bU, float const & bV, float const & arg0, float const & arg1
  ) const {
    float phi = phi_u(bU);
    float rr = r_v(bV);
    float mm = m_v(bV);
    
    return uv_phiRadiusM(phi*rr+arg0, mm+arg1);  
  }
  
  dtPoint2 rotatingMap1dTo3d::uv_phiRadiusM(
    float const & arg0, float const & arg1
  ) const {
    float const vv = v_m(arg1);
    float const rr = r_v(vv);
    float const phi = arg0 / rr;
    return dtPoint2( u_phi(phi), vv );
  }
  
  dtPoint2 rotatingMap1dTo3d::uv_phiM(
    float const & arg0, float const & arg1
  ) const {
    return dtPoint2( u_phi(arg0), v_m(arg1) );
  }
  
  float rotatingMap1dTo3d::zMin( void ) const {
    return z_v( getMin(1) );
  }
  
  float rotatingMap1dTo3d::zMax( void ) const {
    return z_v( getMax(1) );
  }
  
  float rotatingMap1dTo3d::z_v(float const & arg) const {
    //
    //get point in x,y,z coordinates
    //
    dtPoint3 pointXYZ = getPoint(getMin(0), arg);
    //
    //calculate z-coordinate
    //
    return _vv * (pointXYZ - _pp);
  }

  float rotatingMap1dTo3d::v_z(float const & arg) const {
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
  
  float rotatingMap1dTo3d::r_v(float const & arg0) const {
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
  
  float rotatingMap1dTo3d::r_m(float const & arg0) const {
    float const vv = v_m(arg0);
    return r_v(vv);
  }

  rotatingMap1dTo3d * rotatingMap1dTo3d::clone( void ) const {
    return new rotatingMap1dTo3d( *this );
  }

  rotatingMap1dTo3d * rotatingMap1dTo3d::create( void ) const {
    return new rotatingMap1dTo3d();
  }

  rotatingMap1dTo3d::~rotatingMap1dTo3d() {
  }
  
  dtCurve const * rotatingMap1dTo3d::ptrConstRadiusCurve( void ) const {
    map1dTo3d * m1d = segmentConstUPercent(0., 0., 1.);
		dt__ptrAss(
		  splineCurve3d const * sC3,
		  splineCurve3d::ConstDownCast(m1d)
		);
		return sC3->ptrConstDtCurve();
  }
  
//  dtCurve * rotatingMap1dTo3d::getCircleCurve( float const & vv ) const {
//    map1dTo3d * m1d = pickConstVPercent(0., 0., 1.);
//		dt__ptrAss(
//		  splineCurve3d const * sC3,
//		  splineCurve3d::ConstDownCast(m1d)
//		);
//		return sC3->ptrConstDtCurve()->clone();
//    
//  }
	
	void rotatingMap1dTo3d::correctOrigin() {
		dtPoint3 start = map2dTo3d::getPointPercent(0.,0.);
		dtVector3 dist = start - _pp;
		if ( (dist*_vv) != 0. ) {
			float adjusting = (dist*_vv)/sqrt(_vv.squared_length());

			_pp = _pp + _vv  * adjusting;

			dt__warning(correctOrigin(),
							<< dt__eval(dist*_vv) << std::endl 
							<< "Origin of rotSpline is not correct!" << std::endl
							<< "Move origin to " << dt__point3d(_pp) );        
		}
	}
	
  map2dTo3d * rotatingMap1dTo3d::segmentRectangle(
	  dtPoint2 const & p0, dtPoint2 const & p1
  ) const {
		ptrHandling<analyticSurface> aS( 
			analyticSurface::SecureCast(analyticSurface::segmentRectangle(p0, p1)) 
		);
		return new rotatingMap1dTo3d( *(aS.get()->ptrDtSurface()), _pp, _vv, _angle);
  }	
}