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

#include "multipleBoundedVolume.h"
#include <logMe/logMe.h>
#include "map1dTo3d.h"
#include "map2dTo3d.h"
#include <dtTransformerHeaven/dtTransformer.h>
#include <geometryEngine/dtSurface.h>

namespace dtOO {
	multipleBoundedVolume::multipleBoundedVolume() : analyticGeometry() {
	}

	multipleBoundedVolume::multipleBoundedVolume(
    const multipleBoundedVolume& orig
  ) : analyticGeometry(orig) {
		_m3d.reset( orig._m3d->clone() );
    dt__forAllIndex(orig._m2d, ii) _m2d.push_back( orig._m2d[ii].clone() );
	}

	multipleBoundedVolume::~multipleBoundedVolume() {
	}
	
  multipleBoundedVolume::multipleBoundedVolume( 
    analyticGeometry const * const m3d, 
    ptrVectorHandling< analyticGeometry > const & m2d
  ) {
    dt__throwIf( m3d->dim()!=3, multipleBoundedVolume() );
		_m3d.reset( m3d->clone() );
    dt__forAllIndex(m2d, ii) {
      dt__throwIf( m2d[ii].dim()!=2, multipleBoundedVolume() );
      _m2d.push_back( m2d[ii].clone() );    
    }
  }
  
	multipleBoundedVolume * multipleBoundedVolume::create( 
    void 
  ) const {
		return new multipleBoundedVolume();
	}
  
	multipleBoundedVolume * multipleBoundedVolume::clone( void ) const {
		return new multipleBoundedVolume( *this );
	}
  
  dtInt multipleBoundedVolume::dim( void ) const {
    return 3;
	}
  
  bool multipleBoundedVolume::isClosed( dtInt const & dir) const {
    switch (dir) {
      case 0:
        return false;
      case 1:
        return false;
      case 2:
        return false;
      default:
        dt__throwUnexpected(isClosed());
    }
	}

	dtReal multipleBoundedVolume::getMin( dtInt const & dir) const {
    switch (dir) {
      case 0:
        return _m3d->getMin(0);
      case 1:
        return _m3d->getMin(1);
      case 2:
        return _m3d->getMin(2);        
      default:
        dt__throwUnexpected(isClosed());
    }    		
	}
  
	dtReal multipleBoundedVolume::getMax( dtInt const & dir) const {
    switch (dir) {
      case 0:
        return _m3d->getMax(0);
      case 1:
        return _m3d->getMax(1);
      case 2:
        return _m3d->getMax(2);        
      default:
        dt__throwUnexpected(isClosed());
    }    				
	}
    
	dtPoint3 multipleBoundedVolume::getPoint( 
    dtReal const * const uvw 
  ) const {
   	return _m3d->getPoint(uvw);
	}	

  vectorHandling< renderInterface * > 
  multipleBoundedVolume::getRender( void ) const {
		vectorHandling< renderInterface * > rV;
		dt__forAllIndex(_m2d, ii) {
      vectorHandling< renderInterface * > tmpRV = _m2d[ii].getRender(); 
      rV.insert(rV.end(), tmpRV.begin(), tmpRV.end());
    }
    return rV;
  }  

  ptrVectorHandling< analyticGeometry > const & 
  multipleBoundedVolume::boundsVectorConstRef( void ) const {
    return _m2d;
  }
  
  analyticGeometry const * const 
  multipleBoundedVolume::regionConstPtr( void ) const {
    return _m3d.get();
  }  
}
