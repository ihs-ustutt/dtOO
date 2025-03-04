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

#include "pointCurveDist.h"

#include <progHelper.h>
#include <vector>
#include <geometryEngine/dtCurve.h>


namespace dtOO {
  pointCurveDist::pointCurveDist( 
    dtPoint3 const & p3,
    dtCurve const * const dtC
  ) 
  :
  _p3(p3),
  _dimension(1),
  _dtC(dtC)
  {
  }

  pointCurveDist::pointCurveDist( pointCurveDist const & orig ) 
  :
  _p3(orig._p3),
  _dimension(orig._dimension),
  _dtC(orig._dtC)
  {
  }

  pointCurveDist::~pointCurveDist() {
  }

  pointCurveDist * pointCurveDist::clone(void) const {
    return new pointCurveDist(*this);
  }

  dtReal pointCurveDist::operator()( 
    std::vector< dtReal > const & xx 
  ) const {
    return dtLinearAlgebra::distance( _dtC->pointPercent( xx[0] ), _p3 );
  }

  dtInt const & pointCurveDist::dimension() const {
    return _dimension;
  }

  dt__C_addCloneForpVH(pointCurveDist);
}
