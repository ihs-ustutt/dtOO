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

#include "infinityMap3dTo3d.h"
#include <logMe/logMe.h>
#include "map3dTo3dTransformed.h"
#include <dtTransformerHeaven/dtTransformer.h>

namespace dtOO {
	infinityMap3dTo3d::infinityMap3dTo3d() : map3dTo3d() {
    
	}

	infinityMap3dTo3d::infinityMap3dTo3d(
    const infinityMap3dTo3d& orig
  ) : map3dTo3d(orig) {
    
	}

	infinityMap3dTo3d::~infinityMap3dTo3d() {
    
	}
	
	infinityMap3dTo3d * infinityMap3dTo3d::create( void ) const {
		return new infinityMap3dTo3d();
	}
  
	infinityMap3dTo3d * infinityMap3dTo3d::clone( void ) const {
		return new infinityMap3dTo3d( *this );
	}
    
  infinityMap3dTo3d * infinityMap3dTo3d::cloneTransformed( 
    dtTransformer const * const dtT  
  ) const {
    return new map3dTo3dTransformed< infinityMap3dTo3d >(*this, dtT);
  }  
	
  bool infinityMap3dTo3d::isClosed( dtInt const & dir) const {
    switch (dir) {
      case 0:
        return false;
      case 1:
        return false;
      case 2:
        return false;      
      default:
        dt__throw(isClosed(),
              << dt__eval(dir) << std::endl
              << "dir should be 0, 1 or 2.");
    }
	}
  
	dtReal infinityMap3dTo3d::getMin( dtInt const & dir) const {
    switch (dir) {
      case 0:
        return -1.;
      case 1:
        return -1.;
      case 2:
        return -1.;    
      default:
        dt__throw(getMin(),
              << dt__eval(dir) << std::endl
              << "dir should be 0, 1 or 2.");
    }    		
	}
  
	dtReal infinityMap3dTo3d::getMax( dtInt const & dir) const {
    switch (dir) {
      case 0:
        return 1.;
      case 1:
        return 1.;
      case 2:
        return 1.;    
      default:
        dt__throw(getMax(),
              << dt__eval(dir) << std::endl
              << "dir should be 0, 1 or 2.");
    }    				
	}
    
	dtPoint3 infinityMap3dTo3d::getPoint( 
    dtReal const & uu, dtReal const & vv, dtReal const & ww 
  ) const {	
    return dtPoint3(uu, vv, ww);
	}
}
