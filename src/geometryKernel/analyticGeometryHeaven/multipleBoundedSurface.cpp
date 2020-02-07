#include "multipleBoundedSurface.h"
#include <logMe/logMe.h>
#include "map1dTo3d.h"
#include "map2dTo3d.h"
#include <dtTransformerHeaven/dtTransformer.h>
#include <geometryEngine/dtSurface.h>

namespace dtOO {
	multipleBoundedSurface::multipleBoundedSurface() : analyticGeometry() {
	}

	multipleBoundedSurface::multipleBoundedSurface(
    const multipleBoundedSurface& orig
  ) : analyticGeometry(orig) {
		_m2d.reset( orig._m2d->clone() );
    dt__forAllIndex(orig._m1d, ii) _m1d.push_back( orig._m1d[ii].clone() );
	}

	multipleBoundedSurface::~multipleBoundedSurface() {
	}
	
  multipleBoundedSurface::multipleBoundedSurface( 
    analyticGeometry const * const m2d, 
    ptrVectorHandling< analyticGeometry > const & m1d
  ) {
    dt__throwIf( m2d->dim()!=2, multipleBoundedSurface() );
		_m2d.reset( m2d->clone() );
    dt__forAllIndex(m1d, ii) {
      dt__throwIf( m1d[ii].dim()!=1, multipleBoundedSurface() );
      _m1d.push_back( m1d[ii].clone() );    
    }
  }
  
	multipleBoundedSurface * multipleBoundedSurface::create( 
    void 
  ) const {
		return new multipleBoundedSurface();
	}
  
	multipleBoundedSurface * multipleBoundedSurface::clone( void ) const {
		return new multipleBoundedSurface( *this );
	}
  
  int multipleBoundedSurface::dim( void ) const {
    return 2;
	}
  
  bool multipleBoundedSurface::isClosed( int const & dir) const {
    switch (dir) {
      case 0:
        return false;
      case 1:
        return false;
      default:
        dt__throwUnexpected(isClosed());
    }
	}

	float multipleBoundedSurface::getMin( int const & dir) const {
    switch (dir) {
      case 0:
        return _m2d->getMin(0);
      case 1:
        return _m2d->getMin(1);
      default:
        dt__throwUnexpected(isClosed());
    }    		
	}
  
	float multipleBoundedSurface::getMax( int const & dir) const {
    switch (dir) {
      case 0:
        return _m2d->getMax(0);
      case 1:
        return _m2d->getMax(1);
      default:
        dt__throwUnexpected(isClosed());
    }    				
	}
    
	dtPoint3 multipleBoundedSurface::getPoint( 
    float const * const uvw 
  ) const {
   	return _m2d->getPoint(uvw);
	}	

  vectorHandling< renderInterface * > 
  multipleBoundedSurface::getRender( void ) const {
		vectorHandling< renderInterface * > rV;
		dt__forAllIndex(_m1d, ii) {
      vectorHandling< renderInterface * > tmpRV = _m1d[ii].getRender(); 
      rV.insert(rV.end(), tmpRV.begin(), tmpRV.end());
    }

    return rV;
  }  
}
