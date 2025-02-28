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

#include "vectorFunctionDist.h"
#include "dtLinearAlgebra.h"

#include <progHelper.h>
#include <vector>
#include <analyticFunctionHeaven/analyticFunction.h>


namespace dtOO {
  vectorFunctionDist::vectorFunctionDist( 
    std::vector< dtReal > const & vec,
    analyticFunction const * const aF
  ) 
  :
  _vec(vec),
  _dimension(aF->xDim()),
  _aF(aF)
  {
  }

  vectorFunctionDist::vectorFunctionDist( vectorFunctionDist const & orig ) 
  :
  _vec(orig._vec),
  _dimension(orig._dimension),
  _aF(orig._aF)
  {
  }

  vectorFunctionDist::~vectorFunctionDist() {
  }

  vectorFunctionDist * vectorFunctionDist::clone(void) const {
    return new vectorFunctionDist(*this);
  }

  dtReal vectorFunctionDist::operator()( 
    std::vector< dtReal > const & xx 
  ) const {
    return sqrt(
        dtLinearAlgebra::euclidianNorm(
          dtLinearAlgebra::subtract( 
            _aF->Y( _aF->x_percent( aFX(xx) ) ), _vec
          )
        )
    );
  }

  dtInt const & vectorFunctionDist::dimension() const {
    return _dimension;
  }

  dt__C_addCloneForpVH(vectorFunctionDist);
}
