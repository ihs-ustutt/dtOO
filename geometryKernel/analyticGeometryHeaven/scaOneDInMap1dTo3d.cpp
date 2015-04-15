#include "scaOneDInMap1dTo3d.h"
#include <logMe/logMe.h>
#include <analyticFunctionHeaven/scaOneD.h>

namespace dtOO { 
	scaOneDInMap1dTo3d::scaOneDInMap1dTo3d() : map1dTo3d() {

	}

	scaOneDInMap1dTo3d::scaOneDInMap1dTo3d(scaOneD const * const s1d, map1dTo3d const * const m1d) : map1dTo3d() {
		_s1d.reset( s1d->clone() );
		_m1d.reset( m1d->clone() );
		_percentF = false;
	}
	
	scaOneDInMap1dTo3d::scaOneDInMap1dTo3d(scaOneD const * const s1d, map1dTo3d const * const m1d, bool const & percentF) : map1dTo3d() {
		_s1d.reset( s1d->clone() );
		_m1d.reset( m1d->clone() );
		_percentF = percentF;
	}

	scaOneDInMap1dTo3d::scaOneDInMap1dTo3d(const scaOneDInMap1dTo3d& orig) : map1dTo3d(orig) {
		_s1d.reset( orig._s1d->clone() );
		_m1d.reset( orig._m1d->clone() );	
		_percentF = orig._percentF;
	}

	scaOneDInMap1dTo3d::~scaOneDInMap1dTo3d() {

	}

	scaOneDInMap1dTo3d * scaOneDInMap1dTo3d::clone( void ) const {
    return new scaOneDInMap1dTo3d(*this);
	}

	scaOneDInMap1dTo3d * scaOneDInMap1dTo3d::create( void ) const {
		return new scaOneDInMap1dTo3d();
	}

	dtPoint3 scaOneDInMap1dTo3d::getPoint( float const & uu ) const {
		if (_percentF) {
			return _m1d->getPoint( _m1d->u_percent(_s1d->YFloat(uu)) );
		}
		else {
      return _m1d->getPoint( _s1d->YFloat(uu) );
		}
  }

	/**
   * @todo This check is not enough. Maybe a general check on base class?
   */
	bool scaOneDInMap1dTo3d::isClosed( int const & dir) const {
    dt__throwIf(dir != 0, isClosed());
		
		return false;
	}

	float scaOneDInMap1dTo3d::getMin( int const & dir) const {
		return _s1d->xMin(dir);     
	}

	float scaOneDInMap1dTo3d::getMax( int const & dir) const {
		return _s1d->xMax(dir);
	}

  scaOneD const * scaOneDInMap1dTo3d::ptrToScaOneD( void ) const {
	  return _s1d.get();	
	}
	
  map1dTo3d const * scaOneDInMap1dTo3d::ptrToMap1dTo3d( void ) const {
		return _m1d.get();
	}
}