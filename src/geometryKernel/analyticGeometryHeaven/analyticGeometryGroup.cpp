#include "analyticGeometryGroup.h"

#include <logMe/logMe.h>
#include <logMe/dtMacros.h>
#include <discrete3dPoints.h>
#include <discrete3dVector.h>

namespace dtOO {
  analyticGeometryGroup::analyticGeometryGroup() : analyticGeometry() {
  }

  analyticGeometryGroup::analyticGeometryGroup(
    analyticGeometryGroup const & orig
  ) : analyticGeometry(orig) {
    dt__forAllRefAuto(orig._vec, aG) {
      _vec.push_back( aG.clone() );
    }    
  }

  analyticGeometryGroup::analyticGeometryGroup(
    std::vector< analyticGeometry const * > const & vec
  ) : analyticGeometry() {
    dt__forAllRefAuto(vec, aG) {
      _vec.push_back( aG->clone() );
    }
  }
 
  analyticGeometryGroup * analyticGeometryGroup::clone( void ) const {
    return new analyticGeometryGroup( *this );
  }

  analyticGeometryGroup * analyticGeometryGroup::create( void ) const {
    return new analyticGeometryGroup();
  }
	
  analyticGeometryGroup::~analyticGeometryGroup() {
  }

  dtPoint3 analyticGeometryGroup::getPoint( float const * const uvw ) const {
    dt__throwUnexpected(getPoint());
  }
	   
  bool analyticGeometryGroup::isClosed( int const & dir) const {
    return false;
  }
  
  float analyticGeometryGroup::getMin( int const & dir) const {
		return 0.;
  }

  float analyticGeometryGroup::getMax( int const & dir) const {
    return 0.;
  }
  
  int analyticGeometryGroup::dim( void ) const {
    dt__throwUnexpected(dim());
  }  
  
  vectorHandling< renderInterface * > analyticGeometryGroup
    ::getRender( void ) const {
    vectorHandling< renderInterface * > ret;
    dt__forAllRefAuto(_vec, aG) {      
      ret = vectorHandling< renderInterface * >( ret, aG.getRender() );
    }
    
    return ret;
  }

  vectorHandling< renderInterface * > analyticGeometryGroup
    ::getExtRender( void ) const {
    return renderInterface::getExtRender();
  }  
}