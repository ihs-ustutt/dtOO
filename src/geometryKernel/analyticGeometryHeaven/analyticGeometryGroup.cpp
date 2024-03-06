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

  dtPoint3 analyticGeometryGroup::getPoint( dtReal const * const uvw ) const {
    dt__throwUnexpected(getPoint());
  }
	   
  bool analyticGeometryGroup::isClosed( dtInt const & dir) const {
    return false;
  }
  
  dtReal analyticGeometryGroup::getMin( dtInt const & dir) const {
		return 0.;
  }

  dtReal analyticGeometryGroup::getMax( dtInt const & dir) const {
    return 0.;
  }
  
  dtInt analyticGeometryGroup::dim( void ) const {
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
