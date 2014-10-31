#include "curveInSurface.h"
#include <interfaceHeaven/staticPropertiesHandler.h>
#include <logMe/logMe.h>
#include <interfaceHeaven/ptrHandling.h>
#include <geometryEngine/dtCurve2d.h>
#include "map2dTo3d.h"

namespace dtOO { 
	curveInSurface::curveInSurface() : map1dTo3d() {

	}

	curveInSurface::curveInSurface(dtCurve2d const * const dtC2d, map2dTo3d const * const m2d) : map1dTo3d() {
		_dtC2d.reset( dtC2d->clone() );
		_m2d.reset( m2d->clone() );
		_percentF = false;
	}
	
	curveInSurface::curveInSurface(dtCurve2d const * const dtC2d, map2dTo3d const * const m2d, bool const & percentF) : map1dTo3d() {
		_dtC2d.reset( dtC2d->clone() );
		_m2d.reset( m2d->clone() );
		_percentF = percentF;
	}

	curveInSurface::curveInSurface(const curveInSurface& orig) : map1dTo3d(orig) {
    _dtC2d.reset( orig._dtC2d->clone() );
		_m2d.reset( orig._m2d->clone() );	
		_percentF = orig._percentF;
	}

	curveInSurface::~curveInSurface() {

	}

	curveInSurface * curveInSurface::clone( void ) const {
    return new curveInSurface(*this);
	}

	curveInSurface * curveInSurface::create( void ) const {
		return new curveInSurface();
	}

	dtPoint3 curveInSurface::getPoint( float const & uu ) const {
		if (_percentF) {
			return _m2d->getPoint( _m2d->uv_percent(_dtC2d->point(uu)) );
		}
		else {
      return _m2d->getPoint( _dtC2d->point(uu) );
		}
  }

	bool curveInSurface::isClosed( int const & dir) const {
    return _dtC2d->closed();
	}

	float curveInSurface::getMin( int const & dir) const {
    switch (dir) {
      case 0:
        return _dtC2d->minU();
        break;
      default:
        dt__THROW(getMin(),
              << DTLOGEVAL(dir) << LOGDEL
              << "dir should be 0.");
    }       
	}

	float curveInSurface::getMax( int const & dir) const {
    switch (dir) {
      case 0:
        return _dtC2d->maxU();
        break;
      default:
        dt__THROW(getMax(),
              << DTLOGEVAL(dir) << LOGDEL
              << "dir should be 0.");
    }       
	}
}