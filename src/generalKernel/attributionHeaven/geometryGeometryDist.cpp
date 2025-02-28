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

#include "geometryGeometryDist.h"

#include <progHelper.h>
#include <vector>
#include <analyticGeometryHeaven/analyticGeometry.h>


namespace dtOO {
  geometryGeometryDist::geometryGeometryDist( 
    analyticGeometry const * const aG0,
    analyticGeometry const * const aG1
  ) 
  :
  _aG0(aG0),
  _aG1(aG1),
  _dimension(aG0->dim()+aG1->dim())
  {
  }

  geometryGeometryDist::geometryGeometryDist( 
    geometryGeometryDist const & orig 
  ) 
  :
  _aG0(orig._aG0),
  _aG1(orig._aG1),
  _dimension(orig._dimension)
  {
  }

  geometryGeometryDist::~geometryGeometryDist() {
  }

  geometryGeometryDist * geometryGeometryDist::clone(void) const {
    return new geometryGeometryDist(*this);
  }

  dtReal geometryGeometryDist::operator()( 
    std::vector< dtReal > const & xx 
  ) const {
    return dtLinearAlgebra::distance( 
      _aG0->getPointPercent( &(xx[0]) ), 
      _aG1->getPointPercent( &(xx[0+_aG0->dim()]) )
    );
  }

  dtInt const & geometryGeometryDist::dimension() const {
    return _dimension;
  }

  dt__C_addCloneForpVH(geometryGeometryDist);
}
