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

#include "map1dTo3dLength.h"

#include <progHelper.h>
#include <vector>
#include <analyticGeometryHeaven/map1dTo3d.h>


namespace dtOO {
  map1dTo3dLength::map1dTo3dLength( 
    map1dTo3d const * const m1d,
    dtReal const & ll
  ) 
  :
  _m1d(m1d),
  _ll(ll),
  _dimension(1)
  {
  }

  map1dTo3dLength::map1dTo3dLength( map1dTo3dLength const & orig ) 
  :
  _m1d(orig._m1d),
  _ll(orig._ll),
  _dimension(orig._dimension)
  {
  }

  map1dTo3dLength::~map1dTo3dLength() {
  }

  map1dTo3dLength * map1dTo3dLength::clone(void) const {
    return new map1dTo3dLength(*this);
  }

  dtReal map1dTo3dLength::operator()( 
    std::vector< dtReal > const & xx 
  ) const {
    return fabs( _m1d->l_u( _m1d->u_percent(xx[0]) ) - _ll );
  }

  dtInt const & map1dTo3dLength::dimension() const {
    return _dimension;
  }

  dt__C_addCloneForpVH(map1dTo3dLength);
}
