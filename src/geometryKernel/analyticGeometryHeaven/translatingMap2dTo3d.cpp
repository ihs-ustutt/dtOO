#include "translatingMap2dTo3d.h"
#include <logMe/logMe.h>
#include "map3dTo3dTransformed.h"
#include <dtTransformerHeaven/dtTransformer.h>
#include "map2dTo3d.h"
#include "map1dTo3d.h"
#include "analyticCurve.h"
#include "analyticSurface.h"
#include <geometryEngine/dtCurve.h>
#include <geometryEngine/dtSurface.h>
#include <geometryEngine/geoBuilder/geomSurface_surfaceTranslateConstructOCC.h>
#include <geometryEngine/geoBuilder/geomCurve_curveTranslateConstructOCC.h>
#include <geometryEngine/geoBuilder/bSplineSurface_skinConstructOCC.h>
#include <geometryEngine/geoBuilder/bSplineSurface_exchangeSurfaceConstructOCC.h>

namespace dtOO {
	translatingMap2dTo3d::translatingMap2dTo3d() : map3dTo3d() {
	}

	translatingMap2dTo3d::translatingMap2dTo3d(
    const translatingMap2dTo3d& orig
  ) : map3dTo3d(orig) {
		_m2d.reset( orig._m2d->clone() );
		_vv = orig._vv;
	}

	translatingMap2dTo3d::~translatingMap2dTo3d() {
	}
	
  translatingMap2dTo3d::translatingMap2dTo3d( 
    dtVector3 const & vv, map2dTo3d const * const m2d 
  ) : map3dTo3d() {
		if (m2d->isClosedU() || m2d->isClosedV() ) {
			dt__throw(
        translatingMap2dTo3d(),
				<< "Surface is closed. This is not yet supported." << std::endl
				<< dt__eval(m2d->isClosedU()) << std::endl
				<< dt__eval(m2d->isClosedV()) 
      );
		}
		_vv = vv;
		_m2d.reset( m2d->clone() );
	}
  
	translatingMap2dTo3d * translatingMap2dTo3d::create( void ) const {
		return new translatingMap2dTo3d();
	}
  
	translatingMap2dTo3d * translatingMap2dTo3d::clone( void ) const {
		return new translatingMap2dTo3d( *this );
	}
  
  translatingMap2dTo3d * translatingMap2dTo3d::cloneTransformed( 
    dtTransformer const * const dtT  
  ) const {
    return new map3dTo3dTransformed< translatingMap2dTo3d >(*this, dtT);
  }  
	
  bool translatingMap2dTo3d::isClosed( int const & dir) const {
    switch (dir) {
      case 0: return false;
      case 1: return false;
      case 2: return false;
      default:
        dt__throw(
          isClosed(),
          << dt__eval(dir) << std::endl
          << "dir should be 0, 1 or 2."
        );
    }
	}
  
	float translatingMap2dTo3d::getMin( int const & dir) const {
    switch (dir) {
      case 0: return 0.;
      case 1: return _m2d->getUMin();
      case 2: return _m2d->getVMin();
      default:
        dt__throw(
          getMin(),
          << dt__eval(dir) << std::endl
          << "dir should be 0, 1 or 2."
        );
    }    		
	}
  
	float translatingMap2dTo3d::getMax( int const & dir) const {
    switch (dir) {
      case 0: return 1.;
      case 1: return _m2d->getUMax();
      case 2: return _m2d->getVMax();
      default:
        dt__throw(
          getMax(),
          << dt__eval(dir) << std::endl
          << "dir should be 0, 1 or 2."
        );
    }    				
	}
    
	dtPoint3 translatingMap2dTo3d::getPoint( 
    float const & uu, float const & vv, float const & ww 
  ) const {	
		dtAffTransformation3 trans = dtLinearAlgebra::getTranslation( uu * _vv );
		return trans.transform( _m2d->getPoint(vv, ww) );
	}	
	
	map2dTo3d * translatingMap2dTo3d::segmentConstU( float const & uu ) const {
		analyticSurface const * aS = analyticSurface::ConstDownCast(_m2d.get());
		if (aS) {
			dt__pH(dtSurface) dtS(
			  geomSurface_surfaceTranslateConstructOCC(
		      aS->ptrDtSurface(), _vv * uu
			  ).result()
			);
			return new analyticSurface(dtS.get());
		}
		else {
			return map3dTo3d::segmentConstU(uu);
		}
	}

	map2dTo3d * translatingMap2dTo3d::segmentConstV( float const & vv ) const {
		analyticSurface const * aS = analyticSurface::ConstDownCast(_m2d.get());
		if (aS) {
			map1dTo3d * m1d = aS->segmentConstUPercent(percent_v(vv), 0., 1.);
			dt__ptrAss(analyticCurve * s3d, analyticCurve::DownCast(m1d));
      vectorHandling< dtCurve const * > cV(2, NULL);
      cV[0] = s3d->ptrConstDtCurve()->clone();
      cV[1] 
      = 
      geomCurve_curveTranslateConstructOCC( 
        s3d->ptrConstDtCurve(), _vv 
      ).result();        
			dt__pH(dtSurface) dtS(
			  bSplineSurface_skinConstructOCC( cV, 1, 1, 0 ).result()
			);
      cV.destroy();
			dtS.reset(
			  bSplineSurface_exchangeSurfaceConstructOCC( dtS.get() ).result()
			);						
			return new analyticSurface(dtS.get());
		}
		else {
			return map3dTo3d::segmentConstV(vv);
		}
	}
  
	map2dTo3d * translatingMap2dTo3d::segmentConstW( float const & ww ) const {
		analyticSurface const * aS = analyticSurface::ConstDownCast(_m2d.get());
		if (aS) {
			map1dTo3d * m1d = aS->segmentConstVPercent(percent_w(ww), 0., 1.);
			dt__ptrAss(analyticCurve * s3d, analyticCurve::DownCast(m1d));
      vectorHandling< dtCurve const * > cV(2, NULL);
      cV[0] = s3d->ptrConstDtCurve()->clone();
      cV[1] 
      = 
      geomCurve_curveTranslateConstructOCC( 
        s3d->ptrConstDtCurve(), _vv 
      ).result();        
			dt__pH(dtSurface) dtS(
			  bSplineSurface_skinConstructOCC( cV, 1, 1, 0 ).result()
			);
      cV.destroy();
			dtS.reset(
			  bSplineSurface_exchangeSurfaceConstructOCC( dtS.get() ).result()
			);			
			return new analyticSurface(dtS.get());
		}
		else {
			return map3dTo3d::segmentConstW(ww);
		}
	}	
	
  std::string translatingMap2dTo3d::dumpToString( void ) const {
		std::stringstream ss;
		
		ss
    << 
    dt__dumpToString( 
      << "_m2d = " << _m2d->virtualClassName() << std::endl
      << _m2d->dumpToString() << std::endl
		  << dt__vector3d(_vv)     
    );    

		return ss.str();		
	}
  
  map2dTo3d const & translatingMap2dTo3d::constRefMap2dTo3d( void ) const {
    return *(_m2d.get());
  }
  
  dtVector3 const & translatingMap2dTo3d::translationAxis( void ) const {
    return _vv;
  }
}
