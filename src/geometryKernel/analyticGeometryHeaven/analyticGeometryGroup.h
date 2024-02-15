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

#ifndef analyticGeometryGroup_H
#define	analyticGeometryGroup_H

#include <dtOOTypeDef.h>

#include "analyticGeometry.h"
#include <dtLinearAlgebra.h>
#include <vector>

namespace dtOO {
  class dtTransformer;

  class analyticGeometryGroup : public analyticGeometry {
    public:
      dt__class(analyticGeometryGroup, analyticGeometry);    
      analyticGeometryGroup();
      analyticGeometryGroup( analyticGeometryGroup const & orig );
      analyticGeometryGroup( std::vector< analyticGeometry const * > const & vec );
      virtual ~analyticGeometryGroup();
      //
      // overload
      //      
      virtual analyticGeometryGroup * clone( void ) const;      
      virtual analyticGeometryGroup * create( void ) const;
      virtual dtInt dim( void ) const;      
      virtual bool isClosed( dtInt const & dir) const;
      virtual dtReal getMin( dtInt const & dir) const;
      virtual dtReal getMax( dtInt const & dir) const;
      virtual dtPoint3 getPoint( dtReal const * const uvw ) const;      
      virtual vectorHandling< renderInterface * > getRender( void ) const;
      virtual vectorHandling< renderInterface * > getExtRender( void ) const;
    private:
      dt__pVH(analyticGeometry) _vec;
  };
}
#endif	/* analyticGeometryGroup_H */
