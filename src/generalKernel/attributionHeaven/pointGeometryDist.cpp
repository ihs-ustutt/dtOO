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

#include "pointGeometryDist.h"

#include <progHelper.h>
#include <vector>
#include <analyticGeometryHeaven/analyticGeometry.h>


namespace dtOO {
  pointGeometryDist::pointGeometryDist( 
    dtPoint3 const & p3,
    analyticGeometry const * const aG
  ) 
  :
  _p3(p3),
  _dimension(aG->dim()),
  _aG(aG)
  {
  }

  pointGeometryDist::pointGeometryDist( pointGeometryDist const & orig ) 
  :
  _p3(orig._p3),
  _dimension(orig._dimension),
  _aG(orig._aG)
  {
  }

  pointGeometryDist::~pointGeometryDist() {
  }

  pointGeometryDist * pointGeometryDist::clone(void) const {
    return new pointGeometryDist(*this);
  }

  dtReal pointGeometryDist::operator()( 
    std::vector< dtReal > const & xx 
  ) const {
    return dtLinearAlgebra::distance( _aG->getPointPercent( &(xx[0]) ), _p3 );
  }

  dtInt const & pointGeometryDist::dimension() const {
    return _dimension;
  }

  dt__C_addCloneForpVH(pointGeometryDist);
}
