#include "map2dTo3dGroup.h"

#include <logMe/logMe.h>
#include <logMe/dtMacros.h>
#include <discrete3dPoints.h>
#include <discrete3dVector.h>

namespace dtOO {
  map2dTo3dGroup::map2dTo3dGroup() : map2dTo3d() {
  }

  map2dTo3dGroup::map2dTo3dGroup(
    map2dTo3dGroup const & orig
  ) : map2dTo3d(orig) {
    dt__forAllRefAuto(orig._vec, m2d) {
      _vec.push_back( m2d.clone() );
    }    
  }

  map2dTo3dGroup::map2dTo3dGroup(
    std::vector< map2dTo3d const * > const & vec
  ) : map2dTo3d() {
    dt__forAllRefAuto(vec, m2d) {
      _vec.push_back( m2d->clone() );
    }
  }
 
  map2dTo3dGroup * map2dTo3dGroup::clone( void ) const {
    return new map2dTo3dGroup( *this );
  }

  map2dTo3dGroup * map2dTo3dGroup::create( void ) const {
    return new map2dTo3dGroup();
  }
	
	map2dTo3dGroup * map2dTo3dGroup::cloneTransformed( 
    dtTransformer const * const dtT 
  ) const {
		dt__throwUnexpected(cloneTransformed());
	}

  map2dTo3dGroup::~map2dTo3dGroup() {
  }

  dtPoint3 map2dTo3dGroup::getPoint(float const & uu, float const & vv) const {
    dt__throwUnexpected(getPoint());
  }
	   
  bool map2dTo3dGroup::isClosed( int const & dir) const {
    return false;
  }
  
  float map2dTo3dGroup::getMin( int const & dir) const {
		return 0.;
  }

  float map2dTo3dGroup::getMax( int const & dir) const {
    return 0.;
  }
  
  vectorHandling< renderInterface * > map2dTo3dGroup::getRender( void ) const {
    vectorHandling< renderInterface * > ret;
    dt__forAllRefAuto(_vec, aG) {      
      ret = vectorHandling< renderInterface * >( ret, aG.getRender() );
    }
    
    return ret;
  }

  vectorHandling< renderInterface * > map2dTo3dGroup::getExtRender( void ) const {
    return renderInterface::getExtRender();
  }  
}
