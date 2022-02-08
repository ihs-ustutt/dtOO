#include "rotatingMap2dTo3d.h"
#include <logMe/logMe.h>
#include "map3dTo3dTransformed.h"
#include <dtTransformerHeaven/dtTransformer.h>
#include "map2dTo3d.h"
#include "map1dTo3d.h"
#include "analyticCurve.h"
#include "analyticSurface.h"
#include "analyticRotatingMap1dTo3d.h"
#include <geometryEngine/dtSurface.h>
#include <geometryEngine/geoBuilder/surfaceOfRevolution_curveRotateConstructOCC.h>
#include <geometryEngine/dtCurve.h>
#include <geometryEngine/geoBuilder/geomSurface_surfaceRotateConstructOCC.h>

namespace dtOO {
	rotatingMap2dTo3d::rotatingMap2dTo3d() : map3dTo3d() {
	}

	rotatingMap2dTo3d::rotatingMap2dTo3d(
    const rotatingMap2dTo3d& orig
  ) : map3dTo3d(orig) {
		_m2d.reset( orig._m2d->clone() );
		_vv = orig._vv;
		_pp = orig._pp;
	}

	rotatingMap2dTo3d::~rotatingMap2dTo3d() {
	}
	
  rotatingMap2dTo3d::rotatingMap2dTo3d( 
    dtVector3 const & vv, map2dTo3d const * const m2d 
  ) : map3dTo3d() {
		if (m2d->isClosedU() || m2d->isClosedV() ) {
			dt__throw(
        rotatingMap2dTo3d(),
				<< "Surface is closed. This is not yet supported." << std::endl
				<< dt__eval(m2d->isClosedU()) << std::endl
				<< dt__eval(m2d->isClosedV()) 
      );
		}
		_pp = dtPoint3(0.,0.,0.);
		_vv = dtLinearAlgebra::normalize(vv);
		_m2d.reset( m2d->clone() );
		//
		// set origin to default
		//
		correctOrigin();
	}
  
	rotatingMap2dTo3d * rotatingMap2dTo3d::create( void ) const {
		return new rotatingMap2dTo3d();
	}
  
	rotatingMap2dTo3d * rotatingMap2dTo3d::clone( void ) const {
		return new rotatingMap2dTo3d( *this );
	}
  
  rotatingMap2dTo3d * rotatingMap2dTo3d::cloneTransformed( 
    dtTransformer const * const dtT  
  ) const {
    return new map3dTo3dTransformed< rotatingMap2dTo3d >(*this, dtT);
  }  
	
  bool rotatingMap2dTo3d::isClosed( dtInt const & dir) const {
    switch (dir) {
      case 0:
        return true;
      case 1:
        return false;
      case 2:
        return false;
      default:
        dt__throw(
          isClosed(),
          << dt__eval(dir) << std::endl
          << "dir should be 0, 1 or 2."
        );
    }
	}
  
	dtReal rotatingMap2dTo3d::getMin( dtInt const & dir) const {
    switch (dir) {
      case 0:
        return 0.;
      case 1:
        return _m2d->getUMin();
      case 2:
        return _m2d->getVMin();
      default:
        dt__throw(
          getMin(),
          << dt__eval(dir) << std::endl
          << "dir should be 0, 1 or 2."
        );
    }    		
	}
  
	dtReal rotatingMap2dTo3d::getMax( dtInt const & dir) const {
    switch (dir) {
      case 0:
        return 1.;
      case 1:
        return _m2d->getUMax();
      case 2:
        return _m2d->getVMax();
      default:
        dt__throw(
          getMax(),
          << dt__eval(dir) << std::endl
          << "dir should be 0, 1 or 2."
        );
    }    				
	}
    
	dtPoint3 rotatingMap2dTo3d::getPoint( 
    dtReal const & uu, dtReal const & vv, dtReal const & ww 
  ) const {
//    dt__warnIfWithMessage(uu<getUMin(), getPoint(), << uu << " < " << getUMin()); 
//		dt__warnIfWithMessage(vv<getVMin(), getPoint(), << vv << " < " << getVMin());
//		dt__warnIfWithMessage(ww<getWMin(), getPoint(), << ww << " < " << getWMin());
//    dt__warnIfWithMessage(uu>getUMax(), getPoint(), << uu << " > " << getUMax()); 
//		dt__warnIfWithMessage(vv>getVMax(), getPoint(), << vv << " > " << getVMax());
//		dt__warnIfWithMessage(ww>getWMax(), getPoint(), << ww << " > " << getWMax());
		
		dtAffTransformation3 rot = dtLinearAlgebra::getRotation(_vv, uu*2*M_PI);
		return rot.transform( _m2d->getPoint(vv, ww) );
	}	
	
	map2dTo3d * rotatingMap2dTo3d::segmentConstU( dtReal const & uu ) const {
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

	map2dTo3d * rotatingMap2dTo3d::segmentConstV( dtReal const & vv ) const {
		analyticSurface const * aS = analyticSurface::ConstDownCast(_m2d.get());
		if (aS) {
			map1dTo3d * m1d = aS->segmentConstUPercent(percent_v(vv), 0., 1.);
			dt__ptrAss(analyticCurve * s3d, analyticCurve::DownCast(m1d));
			dt__pH(dtSurface) dtS(
			  surfaceOfRevolution_curveRotateConstructOCC(
					*(s3d->ptrConstDtCurve()), _pp, _vv
			  ).result()
			);
			
			return new analyticRotatingMap1dTo3d(*dtS, _pp, _vv);
		}
		else {
			return map3dTo3d::segmentConstV(vv);
		}
	}
  
	map2dTo3d * rotatingMap2dTo3d::segmentConstW( dtReal const & ww ) const {
		analyticSurface const * aS = analyticSurface::ConstDownCast(_m2d.get());
		if (aS) {
			map1dTo3d * m1d = aS->segmentConstVPercent(percent_w(ww), 0., 1.);
			dt__ptrAss(analyticCurve * s3d, analyticCurve::DownCast(m1d));
			dt__pH(dtSurface) dtS(
			  surfaceOfRevolution_curveRotateConstructOCC(
					*(s3d->ptrConstDtCurve()), _pp, _vv
			  ).result()
			);
			
			return new analyticRotatingMap1dTo3d(*dtS, _pp, _vv);
		}
		else {
			return map3dTo3d::segmentConstW(ww);
		}
	}	
	
	void rotatingMap2dTo3d::correctOrigin() {
		dtVector3 dist = _m2d->getPointPercent(0.,0.) - _pp;
		if ( (dist*_vv) != 0. ) {
			dtReal adjusting 
		  = 
			dtLinearAlgebra::dotProduct(dist, _vv)/dtLinearAlgebra::length(_vv);

			_pp = _pp + _vv  * adjusting;

			dt__warning(
        correctOrigin(),
				<< dt__eval(dist*_vv) << std::endl 
				<< "Origin of rotSpline is not correct!" << std::endl
				<< "Move origin to " << dt__point3d(_pp) 
      );
		}
	}

  std::string rotatingMap2dTo3d::dumpToString( void ) const {
		std::stringstream ss;
		
		ss
    << 
    dt__dumpToString( 
      << "_m2d = " << _m2d->virtualClassName() << std::endl
      << _m2d->dumpToString() << std::endl
      << dt__point3d(_pp) << std::endl
		  << dt__vector3d(_vv)     
    );    

		return ss.str();		
	}
  
  map2dTo3d const & rotatingMap2dTo3d::constRefMap2dTo3d( void ) const {
    return *(_m2d.get());
  }
  
  dtVector3 const & rotatingMap2dTo3d::rotationAxis( void ) const {
    return _vv;
  }
    
  dtPoint3 const & rotatingMap2dTo3d::origin( void ) const {
    return _pp;
  }
}
