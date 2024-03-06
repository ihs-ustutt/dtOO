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

#ifndef INFINITYMAP3DTO3D_H
#define	INFINITYMAP3DTO3D_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include "map3dTo3d.h"
#include <logMe/dtMacros.h>

namespace dtOO {
  class map2dTo3d;
  class dtTransformer;
  
  class infinityMap3dTo3d : public map3dTo3d {
  public:
    dt__class(infinityMap3dTo3d, analyticGeometry);
    infinityMap3dTo3d();
    infinityMap3dTo3d(const infinityMap3dTo3d& orig);
    virtual ~infinityMap3dTo3d();
    virtual infinityMap3dTo3d * create( void ) const;    
    virtual infinityMap3dTo3d * clone( void ) const;
    virtual infinityMap3dTo3d * cloneTransformed( 
      dtTransformer const * const dtT 
    ) const;    
    virtual bool isClosed( dtInt const & dir) const;
    virtual dtReal getMin( dtInt const & dir) const;
    virtual dtReal getMax( dtInt const & dir) const;
    virtual dtPoint3 getPoint( 
      dtReal const & uu, dtReal const & vv, dtReal const & ww 
    ) const; 
  };
}
#endif	/* INFINITYMAP3DTO3D_H */
