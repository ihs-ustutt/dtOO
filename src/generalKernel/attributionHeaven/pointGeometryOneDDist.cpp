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

#include "pointGeometryOneDDist.h"

#include <progHelper.h>
#include <vector>
#include <analyticGeometryHeaven/analyticGeometry.h>

namespace dtOO {
  pointGeometryOneDDist::pointGeometryOneDDist( 
    dtReal const & value,
    analyticGeometry const * const aG,
    dtInt const & dir
  ) 
  :
  _value(value),
  _dir(dir),
  _dimension(aG->dim()),
  _aG(aG)
  {
  }

  pointGeometryOneDDist::pointGeometryOneDDist( pointGeometryOneDDist const & orig ) 
  :
  _value(orig._value),
  _dir(orig._dir),
  _dimension(orig._dimension),
  _aG(orig._aG)
  {
  }

  pointGeometryOneDDist::~pointGeometryOneDDist() {
  }

  pointGeometryOneDDist * pointGeometryOneDDist::clone(void) const {
    return new pointGeometryOneDDist(*this);
  }

  dtReal pointGeometryOneDDist::operator()( 
    std::vector< dtReal > const & xx 
  ) const {
    return fabs( 
      _aG->getPointPercent( &(xx[0]) )[_dir] - _value 
    );
  }

  dtInt const & pointGeometryOneDDist::dimension() const {
    return _dimension;
  }

  dt__C_addCloneForpVH(pointGeometryOneDDist);
}
