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

#include "rotatingMap1dTo3d.h"
#include <logMe/logMe.h>
#include "map2dTo3dTransformed.h"
#include <dtTransformerHeaven/dtTransformer.h>
#include "map2dTo3d.h"
#include "map1dTo3d.h"

namespace dtOO {
	rotatingMap1dTo3d::rotatingMap1dTo3d() : map2dTo3d() {
	}

	rotatingMap1dTo3d::rotatingMap1dTo3d(
    const rotatingMap1dTo3d& orig
  ) : map2dTo3d(orig) {
		_m1d.reset( orig._m1d->clone() );
		_vv = orig._vv;
		_pp = orig._pp;
	}

	rotatingMap1dTo3d::~rotatingMap1dTo3d() {
	}
	
  rotatingMap1dTo3d::rotatingMap1dTo3d( 
    dtVector3 const & vv, map1dTo3d const * const m1d 
  ) : map2dTo3d() {
		if (m1d->isClosedU() ) {
			dt__warning(
        rotatingMap1dTo3d(),
				<< "Curve is closed. This is not yet supported." << std::endl
				<< dt__eval(m1d->isClosedU())
      );
		}
		_pp = dtPoint3(0.,0.,0.);
		_vv = dtLinearAlgebra::normalize(vv);
		_m1d.reset( m1d->clone() );
    
		//
		// set origin to default
		//
		correctOrigin();
	}
  
	rotatingMap1dTo3d * rotatingMap1dTo3d::create( void ) const {
		return new rotatingMap1dTo3d();
	}
  
	rotatingMap1dTo3d * rotatingMap1dTo3d::clone( void ) const {
		return new rotatingMap1dTo3d( *this );
	}
  
  rotatingMap1dTo3d * rotatingMap1dTo3d::cloneTransformed( 
    dtTransformer const * const dtT  
  ) const {
    return new map2dTo3dTransformed< rotatingMap1dTo3d >(*this, dtT);
  }  
	
  bool rotatingMap1dTo3d::isClosed( dtInt const & dir) const {
    switch (dir) {
      case 0:
        return true;
      case 1:
        return false;
      default:
        dt__throw(
          isClosed(),
          << dt__eval(dir) << std::endl
          << "dir should be 0 or 1."
        );
    }
	}
  
	dtReal rotatingMap1dTo3d::getMin( dtInt const & dir) const {
    switch (dir) {
      case 0:
        return 0.;
      case 1:
        return _m1d->getUMin();
      default:
        dt__throw(
          getMin(),
          << dt__eval(dir) << std::endl
          << "dir should be 0 or 1."
        );
    }    		
	}
  
	dtReal rotatingMap1dTo3d::getMax( dtInt const & dir) const {
    switch (dir) {
      case 0:
        return 1.;
      case 1:
        return _m1d->getUMax();
      default:
        dt__throw(
          getMax(),
          << dt__eval(dir) << std::endl
          << "dir should be 0 or 1."
        );
    }    				
	}
    
	dtPoint3 rotatingMap1dTo3d::getPoint( 
    dtReal const & uu, dtReal const & vv
  ) const {
		dtAffTransformation3 rot = dtLinearAlgebra::getRotation(_vv, uu*2*M_PI);
		return rot.transform( _m1d->getPoint(vv) );
	}	
		
	void rotatingMap1dTo3d::correctOrigin() {
		dtVector3 dist = _m1d->getPointPercent(0.) - _pp;
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

  std::string rotatingMap1dTo3d::dumpToString( void ) const {
		std::stringstream ss;
		
		ss
    << 
    dt__dumpToString( 
      << "_m1d = " << _m1d->virtualClassName() << std::endl
      << _m1d->dumpToString() << std::endl
      << dt__point3d(_pp) << std::endl
		  << dt__vector3d(_vv)     
    );    

		return ss.str();		
	}
  
  map1dTo3d const & rotatingMap1dTo3d::constRefMap1dTo3d( void ) const {
    return *(_m1d.get());
  }
  
  dtVector3 const & rotatingMap1dTo3d::rotationAxis( void ) const {
    return _vv;
  }
    
  dtPoint3 const & rotatingMap1dTo3d::origin( void ) const {
    return _pp;
  }
}
