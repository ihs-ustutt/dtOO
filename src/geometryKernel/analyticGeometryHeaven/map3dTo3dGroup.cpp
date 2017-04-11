#include "map3dTo3dGroup.h"

#include <logMe/logMe.h>
#include <logMe/dtMacros.h>
#include <discrete3dPoints.h>
#include <discrete3dVector.h>

namespace dtOO {
  map3dTo3dGroup::map3dTo3dGroup() : map3dTo3d() {
  }

  map3dTo3dGroup::map3dTo3dGroup(
    map3dTo3dGroup const & orig
  ) : map3dTo3d(orig) {
    dt__forAllRefAuto(orig._vec, m3d) {
      _vec.push_back( m3d.clone() );
    }    
  }

  map3dTo3dGroup::map3dTo3dGroup(
    std::vector< map3dTo3d const * > const & vec
  ) : map3dTo3d() {
    dt__forAllRefAuto(vec, m3d) {
      _vec.push_back( m3d->clone() );
    }
  }
 
  map3dTo3dGroup * map3dTo3dGroup::clone( void ) const {
    return new map3dTo3dGroup( *this );
  }

  map3dTo3dGroup * map3dTo3dGroup::create( void ) const {
    return new map3dTo3dGroup();
  }
	
	map3dTo3dGroup * map3dTo3dGroup::cloneTransformed( 
    dtTransformer const * const dtT 
  ) const {
		dt__throwUnexpected(cloneTransformed());
	}

  map3dTo3dGroup::~map3dTo3dGroup() {
  }

  dtPoint3 map3dTo3dGroup::getPoint(
    float const & uu, float const & vv, float const & ww 
  ) const {
    dt__throwUnexpected(getPoint());
  }
	   
  bool map3dTo3dGroup::isClosed( int const & dir) const {
    return false;
  }
  
  float map3dTo3dGroup::getMin( int const & dir) const {
		return 0.;
  }

  float map3dTo3dGroup::getMax( int const & dir) const {
    return 0.;
  }
  
  vectorHandling< renderInterface * > map3dTo3dGroup::getRender( void ) const {
    vectorHandling< renderInterface * > ret;
    dt__forAllRefAuto(_vec, aG) {      
      ret = vectorHandling< renderInterface * >( ret, aG.getRender() );
    }
    
    return ret;
  }

  vectorHandling< renderInterface * > map3dTo3dGroup::getExtRender( void ) const {
    return renderInterface::getExtRender();
  }  
}
