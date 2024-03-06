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

#include "analyticRotatingMap1dTo3d.h"

#include <logMe/logMe.h>
#include <interfaceHeaven/staticPropertiesHandler.h>
#include "analyticCurve.h"
#include <logMe/dtMacros.h>
#include <geometryEngine/dtSurface.h>
#include <geometryEngine/dtOCCSurfaceOfRevolution.h>
#include <geometryEngine/dtOCCRectangularTrimmedSurface.h>
#include <geometryEngine/dtCurve.h>
#include <interfaceHeaven/ptrHandling.h>

namespace dtOO {
  analyticRotatingMap1dTo3d::analyticRotatingMap1dTo3d() : analyticSurface() {
  }

  analyticRotatingMap1dTo3d::analyticRotatingMap1dTo3d( 
    analyticRotatingMap1dTo3d const & orig 
  ) : analyticSurface(orig) {
    _pp = orig._pp;
    _vv = orig._vv;
    _angle = orig._angle;
  }

  analyticRotatingMap1dTo3d::analyticRotatingMap1dTo3d(
    dtSurface const & dtS, 
    dtPoint3 const & pp,
    dtVector3 const & vv,
    dtReal const & angle
  ) : analyticSurface(&dtS) {
    dt__throwIf(
      (angle==2.*M_PI)
      &&
      !dtOCCSurfaceOfRevolution::Is(&dtS),
      analyticRotatingMap1dTo3d()
    );
    dt__throwIf(
      (angle!=2.*M_PI)
      &&
      !dtOCCRectangularTrimmedSurface::Is(&dtS),
      analyticRotatingMap1dTo3d()
    );
	  _vv = dtVector3(vv);
		_pp = dtPoint3(pp);
		_angle = angle;		
  }

  analyticRotatingMap1dTo3d::analyticRotatingMap1dTo3d(
    dtSurface const & dtS, 
    dtVector3 const & vv,
    dtReal const & angle
  ) : analyticSurface(&dtS) {
    dt__throwIf(
      (angle==2.*M_PI)
      &&
      !dtOCCSurfaceOfRevolution::Is(&dtS),
      analyticRotatingMap1dTo3d()
    );
    dt__throwIf(
      (angle!=2.*M_PI)
      &&
      !dtOCCRectangularTrimmedSurface::Is(&dtS),
      analyticRotatingMap1dTo3d()
    );
	  _vv = dtVector3(vv);
		_pp = dtPoint3(0,0,0);
		_angle = angle;		
		
		correctOrigin();
  }
  
  dtPoint3 analyticRotatingMap1dTo3d::getOrigin( void ) const {
    return _pp;
  }
  
  dtVector3 analyticRotatingMap1dTo3d::getRotVector( void ) const {
    return _vv;
  }

  dtReal analyticRotatingMap1dTo3d::getAngle( void ) const {
    return _angle;
  }  
      
  dtReal analyticRotatingMap1dTo3d::u_phi(dtReal const & arg) const {
    return 
        map2dTo3d::getUMin() 
      + arg * (
          map2dTo3d::getUMax()-map2dTo3d::getUMin()
        ) 
        / 
        (2. * M_PI);
  }
  
  dtReal analyticRotatingMap1dTo3d::phi_u(dtReal const & arg) const {
    if (arg >= 0.) {
      return 
        2. * M_PI * (arg - map2dTo3d::getUMin()) 
        / 
        (map2dTo3d::getUMax()-map2dTo3d::getUMin());    
    }
    else {
      dtReal uu 
      = 
      2. * M_PI * (fabs(arg) - map2dTo3d::getUMin()) 
      / 
      (map2dTo3d::getUMax()-map2dTo3d::getUMin());
      return (map2dTo3d::getUMax() - uu);
    }
  }
  
  dtReal analyticRotatingMap1dTo3d::v_m(dtReal const & arg) const {
//    ptrHandling<dtCurve const> rr( getRadiusCurve() );
    return ptrConstRadiusCurve()->u_l(arg);
  }
  
  dtReal analyticRotatingMap1dTo3d::m_v(dtReal const & arg) const {
//    ptrHandling<dtCurve const> rr( getRadiusCurve() );
    return ptrConstRadiusCurve()->l_u(arg);    
  }
  
  dtPoint3 analyticRotatingMap1dTo3d::xyz_phiZ(
    dtReal const & arg0, dtReal const & arg1
  ) const {
    return getPoint( u_phi(arg0), v_z(arg1) );
  }
  
  dtPoint2 analyticRotatingMap1dTo3d::uv_phiZ(
    dtReal const & arg0, dtReal const & arg1
  ) const {
    return dtPoint2(u_phi(arg0), v_z(arg1));
  }
  
  dtPoint2 analyticRotatingMap1dTo3d::uv_deltaPhiRadiusDeltaM(
    dtReal const & bU, dtReal const & bV, dtReal const & arg0, dtReal const & arg1
  ) const {
    dtReal phi = phi_u(bU);
    dtReal rr = r_v(bV);
    dtReal mm = m_v(bV);
    
    return uv_phiRadiusM(phi*rr+arg0, mm+arg1);  
  }
  
  dtPoint2 analyticRotatingMap1dTo3d::uv_phiRadiusM(
    dtReal const & arg0, dtReal const & arg1
  ) const {
    dtReal const vv = v_m(arg1);
    dtReal const rr = r_v(vv);
    dtReal const phi = arg0 / rr;
    return dtPoint2( u_phi(phi), vv );
  }
  
  dtPoint2 analyticRotatingMap1dTo3d::uv_phiM(
    dtReal const & arg0, dtReal const & arg1
  ) const {
    return dtPoint2( u_phi(arg0), v_m(arg1) );
  }
  
  dtReal analyticRotatingMap1dTo3d::zMin( void ) const {
    return z_v( getMin(1) );
  }
  
  dtReal analyticRotatingMap1dTo3d::zMax( void ) const {
    return z_v( getMax(1) );
  }
  
  dtReal analyticRotatingMap1dTo3d::z_v(dtReal const & arg) const {
    //
    //get point in x,y,z coordinates
    //
    dtPoint3 pointXYZ = getPoint(getMin(0), arg);
    //
    //calculate z-coordinate
    //
    return _vv * (pointXYZ - _pp);
  }

  dtReal analyticRotatingMap1dTo3d::v_z(dtReal const & arg) const {
    dtReal minZ = zMin();
    dtReal maxZ = zMax();
    
    //
    // iterate
    //
    dtReal vv;
    dtReal relativeTol 
		= 
		staticPropertiesHandler::getInstance()->getOptionFloat("xyz_resolution"); 
    std::vector<dtReal> cR(2);
    std::vector<dtReal> aR(2);
    cR[0] = map2dTo3d::getUMin();
    cR[1] = map2dTo3d::getUMax();
    aR[0] = map2dTo3d::getVMin();
    aR[1] = map2dTo3d::getVMax(); 
    
    vv = aR[0] + .5 * (aR[1]-aR[0]);
    for (int ii=0; ii<100; ii++) {
      dtReal res = fabs(arg-z_v(vv));
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
  
  dtReal analyticRotatingMap1dTo3d::r_v(dtReal const & arg0) const {
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
  
  dtReal analyticRotatingMap1dTo3d::r_m(dtReal const & arg0) const {
    dtReal const vv = v_m(arg0);
    return r_v(vv);
  }

  analyticRotatingMap1dTo3d * analyticRotatingMap1dTo3d::clone( void ) const {
    return new analyticRotatingMap1dTo3d( *this );
  }

  analyticRotatingMap1dTo3d * analyticRotatingMap1dTo3d::create( void ) const {
    return new analyticRotatingMap1dTo3d();
  }

  analyticRotatingMap1dTo3d::~analyticRotatingMap1dTo3d() {
  }
  
  dtCurve const * analyticRotatingMap1dTo3d::ptrConstRadiusCurve( void ) const {
    map1dTo3d * m1d = segmentConstUPercent(0., 0., 1.);
		dt__ptrAss(
		  analyticCurve const * sC3,
		  analyticCurve::ConstDownCast(m1d)
		);
		return sC3->ptrConstDtCurve();
  }
  
	void analyticRotatingMap1dTo3d::correctOrigin() {
		dtPoint3 start = map2dTo3d::getPointPercent(0.,0.);
		dtVector3 dist = start - _pp;
		if ( (dist*_vv) != 0. ) {
			dtReal adjusting = (dist*_vv)/sqrt(_vv.squared_length());

			_pp = _pp + _vv  * adjusting;

			dt__warning(
        correctOrigin(),
				<< dt__eval(dist*_vv) << std::endl 
				<< "Origin of rotSpline is not correct!" << std::endl
				<< "Move origin to " << dt__point3d(_pp) 
      );
		}
	}
	
  map2dTo3d * analyticRotatingMap1dTo3d::segmentRectangle(
	  dtPoint2 const & p0, dtPoint2 const & p1
  ) const {
		ptrHandling<analyticSurface> aS( 
			analyticSurface::SecureCast(analyticSurface::segmentRectangle(p0, p1)) 
		);
		return new analyticRotatingMap1dTo3d( 
      *(aS.get()->ptrDtSurface()), _pp, _vv, _angle
    );
  }	
}
