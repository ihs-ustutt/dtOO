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

#include "partRotatingMap2dTo3d.h"
#include "analyticRotatingMap1dTo3d.h"
#include <geometryEngine/geoBuilder/rectangularTrimmedSurface_curveRotateConstructOCC.h>
#include <geometryEngine/geoBuilder/geomCurve_curveRotateConstructOCC.h>
#include "map3dTo3dTransformed.h"
#include <geometryEngine/dtSurface.h>
#include <geometryEngine/dtCurve.h>

namespace dtOO {
	partRotatingMap2dTo3d::partRotatingMap2dTo3d() : rotatingMap2dTo3d() {
	}

	partRotatingMap2dTo3d::partRotatingMap2dTo3d(
    const partRotatingMap2dTo3d& orig
  ) : rotatingMap2dTo3d(orig) {
    _minB = orig._minB;
    _maxB = orig._maxB;
	}

	partRotatingMap2dTo3d::~partRotatingMap2dTo3d() {
	}
	
  partRotatingMap2dTo3d::partRotatingMap2dTo3d( 
    dtVector3 const & vv, map2dTo3d const * const m2d, 
    dtReal const & minB, dtReal const & maxB
  ) : rotatingMap2dTo3d(vv, m2d) {
    _minB = minB;
    _maxB = maxB;
	}
  
	partRotatingMap2dTo3d * partRotatingMap2dTo3d::create( void ) const {
		return new partRotatingMap2dTo3d();
	}
  
	partRotatingMap2dTo3d * partRotatingMap2dTo3d::clone( void ) const {
		return new partRotatingMap2dTo3d( *this );
	}
  
  partRotatingMap2dTo3d * partRotatingMap2dTo3d::cloneTransformed( 
    dtTransformer const * const dtT  
  ) const {
    return new map3dTo3dTransformed< partRotatingMap2dTo3d >(*this, dtT);
  }  
	
  bool partRotatingMap2dTo3d::isClosed( dtInt const & dir) const {
    switch (dir) {
      case 0:
        return false;
      default:
        return rotatingMap2dTo3d::isClosed( dir );
    }
	}
  
	dtReal partRotatingMap2dTo3d::getMin( dtInt const & dir) const {
    switch (dir) {
      case 0:
        return _minB;
      default:
        return rotatingMap2dTo3d::getMin( dir );
    }    		
	}
  
	dtReal partRotatingMap2dTo3d::getMax( dtInt const & dir) const {
    switch (dir) {
      case 0:
        return _maxB;
      default:
        return rotatingMap2dTo3d::getMax( dir );
    }    		
	}
    
	dtPoint3 partRotatingMap2dTo3d::getPoint( 
    dtReal const & uu, dtReal const & vv, dtReal const & ww 
  ) const {
	  return rotatingMap2dTo3d::getPoint( uu, vv, ww );	
	}

 	map2dTo3d * partRotatingMap2dTo3d::segmentConstV( dtReal const & vv ) const {
		if ( analyticSurface::Is( rotatingMap2dTo3d::constPtrMap2dTo3d() ) ) {
			dt__pH(analyticCurve) s3d( 
        analyticCurve::SecureCast(  
          analyticSurface::ConstDownCast(
            rotatingMap2dTo3d::constPtrMap2dTo3d()
          )->segmentConstUPercent(percent_v(vv))
        )
      );

      // 
      // surface is created in two steps:
      //  1) rotate curve by _minB
      //  2) create rectangularTrimmedSurface with rotated curve and with an
      //     angle of 2.*M_PI*(_maxB-_minB)
      //
			return new analyticRotatingMap1dTo3d(
        *dt__pH(dtSurface)(
	        rectangularTrimmedSurface_curveRotateConstructOCC(
            *dt__pH(dtCurve)(
              geomCurve_curveRotateConstructOCC(
                s3d->ptrConstDtCurve(),
                origin(),
                rotationAxis(),
                2.0*M_PI * _minB
              ).result()
            ),
            origin(), 
            rotationAxis(), 
            2.0*M_PI * (_maxB - _minB)
          ).result()
        ),
        origin(), 
        rotationAxis(), 
        2.0*M_PI * (_maxB - _minB)
      );
		}
		else return map3dTo3d::segmentConstV(vv);
	}
  
	map2dTo3d * partRotatingMap2dTo3d::segmentConstW( dtReal const & ww ) const {
		if ( analyticSurface::Is( rotatingMap2dTo3d::constPtrMap2dTo3d() ) ) {
			dt__pH(analyticCurve) s3d( 
        analyticCurve::SecureCast(  
          analyticSurface::ConstDownCast(
            rotatingMap2dTo3d::constPtrMap2dTo3d()
          )->segmentConstVPercent(percent_w(ww))
        )
      );

      // 
      // surface is created in two steps:
      //  1) rotate curve by _minB
      //  2) create rectangularTrimmedSurface with rotated curve and with an
      //     angle of 2.*M_PI*(_maxB-_minB)
      //
			return new analyticRotatingMap1dTo3d(
        *dt__pH(dtSurface)(
	        rectangularTrimmedSurface_curveRotateConstructOCC(
            *dt__pH(dtCurve)(
              geomCurve_curveRotateConstructOCC(
                s3d->ptrConstDtCurve(),
                origin(),
                rotationAxis(),
                2.0*M_PI * _minB
              ).result()
            ),
            origin(), 
            rotationAxis(), 
            2.0*M_PI * (_maxB - _minB)
          ).result()
        ),
        origin(), 
        rotationAxis(), 
        2.0*M_PI * (_maxB - _minB)
      );
		}
		else return map3dTo3d::segmentConstW(ww);
	}	
}
